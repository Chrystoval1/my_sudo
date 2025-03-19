/*
** EPITECH PROJECT, 2025
** main
** File description:
** main file for the project my_sudo
*/

#include "../include/my_sudo.h"
#include <unistd.h>

static void free_struct(sudo_t *sudo_params)
{
    if (sudo_params->name != NULL)
        free(sudo_params->name);
    if (sudo_params->asker_name != NULL)
        free(sudo_params->asker_name);
    if (sudo_params->shell != NULL)
        free(sudo_params->shell);
    if (sudo_params->shell_command != NULL)
        free(sudo_params->shell_command);
    if (sudo_params->group != NULL)
        free(sudo_params->group);
    if (sudo_params != NULL)
        free(sudo_params);
}

static int show_help(int argc, sudo_t *sudo_params)
{
    printf("usage: ./my_sudo -h\n"
        "usage: ./my_sudo [-ugEs] [command [args ...]]\n");
    free_struct(sudo_params);
    if (argc <= 1)
        return 84;
    return 0;
}

static int exec_sudo(sudo_t *sudo_params)
{
    char *path;
    extern char **environ;
    char **shell_c = NULL;

    if (sudo_params->env_preserve == 1)
        sudo_params->env = environ;
    else
        sudo_params->env = create_env_default();
    if (sudo_params->shell_option == 0) {
        path = get_command_path(sudo_params->argv[sudo_params->start_exec]);
        execve(path, &sudo_params->argv[sudo_params->start_exec],
            sudo_params->env);
        if (path != NULL)
            free(path);
    } else {
        shell_c = get_shell_command(sudo_params);
        execve(sudo_params->shell, shell_c, sudo_params->env);
        free_shell_command(shell_c);
    }
    return 84;
}

static int my_sudo(sudo_t *sp)
{
    if (verif_sudo_and_check_password(sp) == 84)
        return 84;
    if (show_list(sp) == 1)
        return 0;
    set_all_uid_and_gid(sp);
    set_shell(sp);
    if (exec_sudo(sp) == 84) {
        fprintf(stderr, "my_sudo: %s: command not found\n",
            sp->argv[sp->start_exec]);
        if (sp->env_preserve == 0)
            free_shell_command(sp->env);
        return 84;
    }
    free_shell_command(sp->env);
    return 0;
}

int is_correct_password(sudo_t *sp)
{
    char *password;
    char *hash;

    if (getuid() == 0)
        return 1;
    password = ask_password(sp);
    hash = get_hash_from_name(sp->asker_name);
    while (check_password(password, hash, sp) == 0 && sp->tries < 3) {
        sleep(2);
        printf("Sorry, try again.\n");
        password = ask_password(sp);
    }
    free(password);
    free(hash);
    if (sp->tries >= 3) {
        sleep(2);
        printf("my_sudo: 3 incorrect password attempts\n");
        return 0;
    }
    return 1;
}

int verifiy_sudo(sudo_t *sudo_params)
{
    if (!is_user_valid(sudo_params))
        return 0;
    return 1;
}

int main(int argc, char **argv, char **env)
{
    sudo_t *sudo_params = get_sudo_params(argc, argv);

    if (argc <= 1)
        return show_help(argc, sudo_params);
    if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0))
        return show_help(argc, sudo_params);
    if (sudo_params == NULL || ((sudo_params->start_exec >= argc) &&
        !sudo_params->shell_option && !sudo_params->logging_option)) {
        free_struct(sudo_params);
        return 84;
    }
    if (my_sudo(sudo_params) == 84) {
        free_struct(sudo_params);
        return 84;
    }
    free_struct(sudo_params);
    return 0;
}
