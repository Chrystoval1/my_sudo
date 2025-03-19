/*
** EPITECH PROJECT, 2025
** src/my_sudo_sudoers
** File description:
** file where the sudoers file is checked
*/

#include "../include/my_sudo.h"

static int is_number(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

int is_same_person(char *id, char *buff)
{
    if (id == NULL || buff == NULL)
        return 0;
    if (strncmp(buff, id, strlen(id)) == 0)
        return 1;
    if (buff[0] == '#' && buff[1] != ' ' && is_number(buff[1])) {
        if (get_uid(id) == atoi(&buff[1]))
            return 1;
    }
    return 0;
}

int is_same_group(char *id, char **groups, char *buff, int i)
{
    if (strncmp(groups[i], &buff[1], strlen(groups[i])) == 0)
        return 1;
    if (buff[0] == '%' && buff[1] == '#' && is_number(buff[2])) {
        if (get_gid(groups[i]) == atoi(&buff[2]))
            return 1;
    }
    return 0;
}

int check_id_in_sudoers(char *id)
{
    FILE *file;
    char buff[500];
    int size = 500;
    int is_in_sudoers = 0;

    file = fopen("/etc/sudoers", "r");
    if (file == NULL || id == NULL)
        return -1;
    while (fgets(buff, size, file)) {
        remove_newline(buff);
        if (is_same_person(id, buff)) {
            is_in_sudoers = 1;
            break;
        }
    }
    if (is_in_sudoers == 0)
        return -1;
    fclose(file);
    return 0;
}

int check_group(char *id, char *buff)
{
    char **groups = get_all_groups(id);

    if (buff == NULL || id == NULL || groups == NULL)
        return 84;
    for (int i = 0; groups[i] != NULL; i++) {
        if (is_same_group(id, groups, buff, i))
            return 1;
    }
    return 0;
}

int check_group_in_sudoers(char *id)
{
    FILE *file;
    char buff[500];
    int size = 500;
    int in_sudoers = 0;

    file = fopen("/etc/sudoers", "r");
    if (file == NULL || id == NULL)
        return 84;
    while (fgets(buff, size, file))
        if (check_group(id, buff) == 1)
            in_sudoers = 1;
    if (in_sudoers == 0)
        return -1;
    fclose(file);
    return in_sudoers;
}
