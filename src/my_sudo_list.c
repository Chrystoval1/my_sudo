/*
** EPITECH PROJECT, 2025
** src/my_sudo_logging
** File description:
** This file handle the -l flag
*/

#include "../include/my_sudo.h"
#include <stdlib.h>

static int get_amount_char_until_eq(char *buff)
{
    int count = 0;

    while (buff[count] != '=')
        count++;
    return count;
}

static void get_all_id(char *id)
{
    FILE *file;
    char buff[500];
    int size = 500;

    file = fopen("/etc/sudoers", "r");
    if (file == NULL || id == NULL)
        return;
    while (fgets(buff, size, file)) {
        remove_newline(buff);
        if (is_same_person(id, buff)) {
            printf("    %s\n", &buff[get_amount_char_until_eq(buff) + 1]);
        }
    }
    fclose(file);
}

static void get_all_group(char *id)
{
    FILE *file;
    char buff[500];
    int size = 500;

    file = fopen("/etc/sudoers", "r");
    if (file == NULL || id == NULL)
        return;
    while (fgets(buff, size, file))
        if (check_group(id, buff) == 1) {
            printf("    %s", &buff[get_amount_char_until_eq(buff) + 1]);
        }
    fclose(file);
}

static void show_default(char *buff, int count)
{
    if (count == 0)
        printf("    %s", &buff[strlen("Defaults") + 1]);
    else
        printf(", %s", &buff[strlen("Defaults") + 1]);
}

static void print_sudoers_defaults(void)
{
    FILE *file = fopen("/etc/sudoers", "r");
    char buff[512];
    int count = 0;

    while (fgets(buff, sizeof(buff), file)) {
        remove_newline(buff);
        if (strncmp(buff, "Defaults", strlen("Defaults")) == 0) {
            show_default(buff, count);
            count++;
        }
    }
    fflush(stdout);
    write(1, "\n\n", 2);
    fclose(file);
}

int show_list(sudo_t *sp)
{
    if (sp->logging_option == 0)
        return 0;
    if (check_id_in_sudoers(sp->asker_name) == -1 &&
        check_group_in_sudoers(sp->asker_name) == -1 && getuid() != 0) {
        printf("Sorry, user %s may not run sudo on %s.\n",
            sp->asker_name, getenv("HOSTNAME"));
        return 1;
    }
    printf("Matching Defaults entries for %s on %s:\n", sp->asker_name,
        getenv("HOSTNAME"));
    print_sudoers_defaults();
    printf("User %s may run the following commands on %s:\n",
        sp->asker_name, getenv("HOSTNAME"));
    get_all_id(sp->asker_name);
    get_all_group(sp->asker_name);
    return 1;
}
