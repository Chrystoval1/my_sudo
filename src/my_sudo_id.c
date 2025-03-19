/*
** EPITECH PROJECT, 2025
** src/my_sudo_id
** File description:
** file where the ids are managed
*/

#include "../include/my_sudo.h"

int get_uid(char *id)
{
    FILE *fd;
    char buff[512];
    char str[strlen(id) + 256];
    int uid = -1;

    if (id[0] == '#')
        return atoi(&id[1]);
    fd = fopen("/etc/passwd", "r");
    strcpy(str, id);
    strcat(str, ":%*c:%i");
    while (fgets(buff, sizeof(buff), fd)) {
        if (strncmp(buff, id, strlen(id)) == 0) {
            sscanf(buff, str, &uid);
            break;
        }
    }
    fclose(fd);
    return uid;
}

int get_gid_in_passwd(char *id)
{
    FILE *fd;
    char buff[512];
    char str[strlen(id) + 256];
    int uid = -1;

    if (id[0] == '#')
        return atoi(&id[1]);
    fd = fopen("/etc/passwd", "r");
    strcpy(str, id);
    strcat(str, ":%*c:%*i:%i");
    while (fgets(buff, sizeof(buff), fd)) {
        if (strncmp(buff, id, strlen(id)) == 0) {
            sscanf(buff, str, &uid);
            break;
        }
    }
    fclose(fd);
    return uid;
}

int get_gid(char *id)
{
    FILE *fd;
    char buff[512];
    int res = -1;
    char str[10];
    char temp[strlen(id) + 7];

    if (id[0] == '#')
        return atoi(&id[1]);
    fd = fopen("/etc/group", "r");
    strcpy(temp, id);
    strcat(temp, ":%*c:%i");
    while (fgets(buff, sizeof(buff), fd)) {
        if (strncmp(buff, id, strlen(id)) == 0) {
            sscanf(buff, temp, &res);
            break;
        }
    }
    fclose(fd);
    return res;
}
