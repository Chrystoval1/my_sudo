/*
** EPITECH PROJECT, 2025
** src/my_sudo_shell
** File description:
** This file contains function related for creating the shell
*/

#include "../include/my_sudo.h"
#include <stdio.h>

static char *strcat_malloc(char *dest, const char *src)
{
    int len_dest = dest ? strlen(dest) : 0;
    int len_src = strlen(src);
    int new_len = len_dest + len_src + 1;
    char *new_str = malloc(new_len);

    if (!new_str)
        return NULL;
    if (dest) {
        strcpy(new_str, dest);
        free(dest);
    } else
        new_str[0] = '\0';
    strcat(new_str, src);
    return new_str;
}

int set_shell(sudo_t *sudo_params)
{
    FILE *file;
    char buff[500];
    int size = 500;

    file = fopen("/etc/shells", "r");
    if (file == NULL || sudo_params->shell == NULL)
        return -1;
    while (fgets(buff, size, file)) {
        remove_newline(buff);
        if (strncmp(buff, sudo_params->shell, strlen(sudo_params->shell)) != 0)
            continue;
        setenv("SHELL", sudo_params->shell, 1);
        break;
    }
    fclose(file);
    return 0;
}

void free_shell_command(char **shell_c)
{
    int i = 0;

    if (shell_c == NULL)
        return;
    while (shell_c[i] != NULL) {
        free(shell_c[i]);
        i++;
    }
    free(shell_c);
}

char **get_shell_command(sudo_t *sudo_params)
{
    char **shell_c = malloc(sizeof(char *) * 4);

    shell_c[0] = strdup(sudo_params->shell);
    if (sudo_params->shell_command == NULL ||
        sudo_params->shell_command[0] == '\0') {
        shell_c[1] = NULL;
        return shell_c;
    }
    shell_c[1] = strdup("-c");
    shell_c[2] = NULL;
    shell_c[3] = NULL;
    for (int i = sudo_params->start_exec; i < sudo_params->argc; i++) {
        shell_c[2] = strcat_malloc(shell_c[2], sudo_params->argv[i]);
        shell_c[2] = strcat_malloc(shell_c[2], " ");
    }
    return shell_c;
}
