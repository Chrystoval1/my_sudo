/*
** EPITECH PROJECT, 2025
** src/my_sudo_parser
** File description:
** Contains all functions for parsing the sudo command
*/

#include "../include/my_sudo.h"
#include <stdlib.h>
#include <unistd.h>

static int set_u_flag(sudo_t *sudo_params, int argc, char *argv[])
{
    int is_flag = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-u") != 0 && strcmp(argv[i], "--user") != 0)
            continue;
        sudo_params->as_user_option = 1;
        is_flag = 1;
        if (argv[i + 1] == NULL || argv[i + 1][0] == '-' ||
            argv[i + 1][0] == '\0') {
            sudo_params->should_quit = 1;
            return 0;
        }
        sudo_params->name = strdup(argv[i + 1]);
        sudo_params->start_exec += 2;
        return 1;
    }
    sudo_params->name = strdup("root");
    return !is_flag;
}

static void set_g_flag(sudo_t *sudo_params, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-g") != 0 && strcmp(argv[i], "--group") != 0)
            continue;
        if (argv[i + 1] == NULL || argv[i + 1][0] == '-'
            || get_gid(argv[i + 1]) == -1) {
            sudo_params->should_quit = 1;
            return;
        }
        sudo_params->group = strdup(argv[i + 1]);
        sudo_params->start_exec += 2;
        return;
    }
    sudo_params->group = NULL;
}

static void set_shell_default(int valid_s, sudo_t *sudo_params)
{
    passwd_t *p;

    if (sudo_params->name == NULL || sudo_params->name[0] == '\0')
        return;
    p = get_passwd(sudo_params->name);
    if (valid_s)
        if (getenv("SHELL") != NULL)
            sudo_params->shell = strdup(getenv("SHELL"));
    if (sudo_params->shell == NULL || sudo_params->shell[0] == '\0')
        if (p->shell[0] != '\0')
            sudo_params->shell = strdup(p->shell);
    if (sudo_params->shell == NULL || sudo_params->shell[0] == '\0')
        sudo_params->shell = strdup("/bin/sh");
    free_passwd(p);
}

static void set_s_flag(sudo_t *sudo_params, int argc, char *argv[])
{
    int valid_s = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-s") != 0 && strcmp(argv[i], "--shell"))
            continue;
        valid_s = 1;
        sudo_params->shell_option = 1;
        if (argv[i + 1] == NULL)
            break;
        if (argv[i + 1][0] == '-')
            break;
        valid_s = 0;
        sudo_params->shell_command = strdup(argv[i + 1]);
        sudo_params->start_exec += 1;
        break;
    }
    set_shell_default(valid_s, sudo_params);
}

static int is_flag(sudo_t *sudo_params, int argc, char *argv[], int i)
{
    int flag = 0;

    for (int j = 1; j < strlen(argv[i]); j++) {
        if (argv[i][j] == 'E') {
            flag = 1;
            sudo_params->env_preserve = 1;
            continue;
        }
        if (argv[i][j] == 'l') {
            sudo_params->logging_option = 1;
            flag = 1;
        }
        if (argv[i][j] == 's')
            return -1;
        if (argv[i][j] != 'E' && argv[i][j] != 's' && argv[i][j] != 'u'
            && argv[i][j] != 'g' && argv[i][j] != 'l')
            return 84;
    }
    return flag;
}

static void set_flags(sudo_t *sudo_params, int argc, char *argv[])
{
    int temp_value;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--group") == 0 ||
            strcmp(argv[i], "--shell") == 0 || strcmp(argv[i], "--user") == 0)
            continue;
        if (argv[i][0] != '-')
            continue;
        temp_value = is_flag(sudo_params, argc, argv, i);
        if (temp_value == -1)
            break;
        if (temp_value == 1)
            sudo_params->start_exec += 1;
        if (temp_value == 84) {
            sudo_params->should_quit = 1;
            return;
        }
    }
}

static void init_defaults(sudo_t *sudo_params, int argc, char *argv[])
{
    sudo_params->start_exec = 1;
    sudo_params->env_preserve = 0;
    sudo_params->logging_option = 0;
    sudo_params->shell_option = 0;
    sudo_params->tries = 0;
    sudo_params->shell = NULL;
    sudo_params->shell_command = NULL;
    sudo_params->group = NULL;
    sudo_params->asker_name = NULL;
    sudo_params->name = NULL;
    sudo_params->env = NULL;
    sudo_params->argv = argv;
    sudo_params->argc = argc;
    sudo_params->uid_to_be = -1;
    sudo_params->should_quit = 0;
    sudo_params->as_user_option = 0;
}

char *get_asker_name(int uid)
{
    FILE *fd;
    char buff[512];
    char asker_name[256];
    char pass[256];
    int asker_uid;

    fd = fopen("/etc/passwd", "r");
    while (fgets(buff, sizeof(buff), fd)) {
        sscanf(buff, "%31[^:]:%31[^:]:%d:", asker_name, pass, &asker_uid);
        if (asker_uid == uid)
            break;
    }
    fclose(fd);
    return strdup(asker_name);
}

sudo_t *get_sudo_params(int argc, char *argv[])
{
    sudo_t *sudo_params = malloc(sizeof(sudo_t));

    if (sudo_params == NULL)
        return NULL;
    init_defaults(sudo_params, argc, argv);
    sudo_params->asker_name = get_asker_name(getuid());
    set_u_flag(sudo_params, argc, argv);
    set_g_flag(sudo_params, argc, argv);
    set_flags(sudo_params, argc, argv);
    set_s_flag(sudo_params, argc, argv);
    if (sudo_params->name != NULL && sudo_params->name[0] != '\0')
        sudo_params->uid_to_be = get_uid(sudo_params->name);
    sudo_params->groups = get_all_groups(sudo_params->asker_name);
    return sudo_params;
}

/*
 * DEBUG ONLY
 */
void show_parser(sudo_t *sudo_params)
{
    printf("\033[31m=============== Sudo Values ===============\033[0m\n");
    printf("Asker NAME : %s\n", sudo_params->asker_name);
    printf("Asker UID : %d\n", getuid());
    printf("-u : %s\n", sudo_params->name);
    printf("-u UID : %d\n", sudo_params->uid_to_be);
    printf("-g : %s\n", sudo_params->group);
    printf("-E : %d\n", sudo_params->env_preserve);
    printf("-s : %s\n", sudo_params->shell);
    printf("-c(s) : %s\n", sudo_params->shell_command);
    printf("-l ? : %d\n", sudo_params->logging_option);
    print_groups(sudo_params->groups);
    printf("Start Exec : %d\n", sudo_params->start_exec);
    printf("Argv : \n");
    for (int i = sudo_params->start_exec; i < sudo_params->argc; i++) {
        printf("- %s\n", sudo_params->argv[i]);
    }
    printf("\033[31m===========================================\033[0m\n\n");
}
