/*
** EPITECH PROJECT, 2025
** src/my_sudo_utils
** File description:
** utilitary file for my_sudo
*/

#include "../include/my_sudo.h"
#include <stdio.h>
#include <stdlib.h>

static passwd_t *init_passwd(void)
{
    passwd_t *p = malloc(sizeof(passwd_t));

    p->uid = 0;
    p->gid = 0;
    p->home = init_empty_string(256);
    p->pass = init_empty_string(256);
    p->user = init_empty_string(256);
    p->shell = init_empty_string(256);
    p->comment = init_empty_string(256);
    return p;
}

static int scanf_passwd(passwd_t *p, char *buff, int search_uid)
{
    int uid;

    if (sscanf(buff, "%31[^:]:%31[^:]:%d:%d:%127[^:]:%127[^:]:%127[^\n]",
        p->user, p->pass, &p->uid, &p->gid,
        p->comment, p->home, p->shell) == 7)
        return 1;
    if (sscanf(buff, "%31[^:]:%31[^:]:%d:%d::%127[^:]:%127[^\n]",
        p->user, p->pass, &p->uid, &p->gid, p->home, p->shell) == 6)
        return 1;
    return -1;
}

static char *find_binary_in_paths(char *path_var, char *command)
{
    char *path_copy = strdup(path_var);
    char *dir = strtok(path_copy, ":");
    char *result = NULL;
    char full_path[4096];

    while (dir) {
        snprintf(full_path, 4096, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0) {
            result = strdup(full_path);
            break;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return result;
}

char *get_command_path(char *command)
{
    char *path = getenv("PATH");

    if (command != NULL && (command[0] == '/' || command[0] == '.'))
        return strdup(command);
    if (!path)
        return NULL;
    return find_binary_in_paths(path, command);
}

void free_passwd(passwd_t *p)
{
    if (p == NULL)
        return;
    if (p->home != NULL)
        free(p->home);
    if (p->pass != NULL)
        free(p->pass);
    if (p->user != NULL)
        free(p->user);
    if (p->shell != NULL)
        free(p->shell);
    if (p->comment != NULL)
        free(p->comment);
    free(p);
}

passwd_t *get_passwd(char *id)
{
    FILE *fd;
    char buff[512];
    passwd_t *p = init_passwd();
    int search_uid = -1;

    if (id[0] == '#')
        search_uid = atoi(id + 1);
    fd = fopen("/etc/passwd", "r");
    while (fgets(buff, sizeof(buff), fd)) {
        if (search_uid == -1 && strncmp(buff, id, strlen(id)) != 0)
            continue;
        if (scanf_passwd(p, buff, search_uid) == 1)
            break;
    }
    fclose(fd);
    return p;
}
