/*
** EPITECH PROJECT, 2025
** src/my_sudo_groups
** File description:
** This file contains functions for getting the groups of a user
*/

#include "../include/my_sudo.h"
#include <stdlib.h>
#include <unistd.h>

static void add_in_array(char ***array, const char *str)
{
    int count = 0;
    char **new_array;

    if (*array != NULL)
        while ((*array)[count] != NULL)
            count++;
    new_array = malloc((count + 2) * sizeof(char *));
    for (int i = 0; i < count; i++)
        new_array[i] = (*array)[i];
    new_array[count] = strdup(str);
    new_array[count + 1] = NULL;
    free(*array);
    *array = new_array;
}

void print_groups(char **groups)
{
    if (groups == NULL) {
        printf("No groups.\n");
        return;
    }
    printf("[ ");
    for (int i = 0; groups[i] != NULL; i++) {
        printf("\"%s\"", groups[i]);
        if (groups[i + 1] != NULL)
            printf(", ");
    }
    printf(" ]\n");
}

static void get_all_groups_in_line(char *temp, char ***array,
    char *id, char *buff)
{
    char *temp_2;

    temp_2 = strtok(temp, ",");
    while (temp_2 != NULL) {
        if (strcmp(id, temp_2) == 0)
            add_in_array(array, strtok(buff, ":"));
        temp_2 = strtok(NULL, ",");
    }
}

char **get_all_groups(char *id)
{
    FILE *fd;
    char buff[1024];
    char **array = NULL;
    char *temp;

    fd = fopen("/etc/group", "r");
    while (fgets(buff, sizeof(buff), fd)) {
        strtok(buff, ":");
        strtok(NULL, ":");
        strtok(NULL, ":");
        temp = strtok(NULL, ":");
        remove_newline(temp);
        get_all_groups_in_line(temp, &array, id, buff);
    }
    fclose(fd);
    return array;
}

static void set_group_passwd(char *id)
{
    gid_t group[1] = {get_gid_in_passwd(id)};

    setgroups(1, group);
}

void set_all_groups(char *id)
{
    char **groups = get_all_groups(id);
    int num = 0;
    gid_t *group;
    int i = 0;

    if (groups == NULL) {
        set_group_passwd(id);
        return;
    }
    while (groups[i] != NULL) {
        num++;
        i++;
    }
    group = malloc(sizeof(gid_t) * (num + 1));
    group[num] = get_gid_in_passwd(id);
    for (int i = 0; i < num; i++)
        group[i] = get_gid(groups[i]);
    setgroups(num + 1, group);
}
