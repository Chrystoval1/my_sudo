/*
** EPITECH PROJECT, 2025
** src/my_sudo_verification
** File description:
** This file will verify if some argument is correct or no
*/

#include "../include/my_sudo.h"
#include <stdio.h>

/*
 * Check if the user name passed by -u is valid or not.
 * Check if passed with # or not.
 */
int is_user_valid(sudo_t *sudo_params)
{
    passwd_t *p = get_passwd(sudo_params->name);

    if (p == NULL)
        return 0;
    if (p->user[0] == '\0') {
        free_passwd(p);
        return 0;
    }
    free_passwd(p);
    return 1;
}

void set_all_uid_and_gid(sudo_t *sp)
{
    if (get_gid(sp->name) != 0 ||
        (get_gid(sp->name) != 0 && sp->group != NULL))
        set_all_groups(sp->name);
    if (get_gid(sp->name) == 0 || sp->group != NULL)
        set_all_groups(sp->name);
    if (sp->group != NULL)
        setgid(get_gid(sp->group));
    else
        setgid(get_gid_in_passwd(sp->name));
    setuid(sp->uid_to_be);
    if (sp->group != NULL && sp->as_user_option == 0) {
        set_all_groups(sp->asker_name);
        setuid(get_uid(sp->asker_name));
    }
}

int verif_sudo_and_check_password(sudo_t *sp)
{
    if (sp->should_quit == 1)
        return 84;
    if (verifiy_sudo(sp) == 0)
        return 84;
    if (is_correct_password(sp) == 0)
        return 84;
    if (getuid() != 0 &&
        check_id_in_sudoers(sp->asker_name) == -1 &&
        check_group_in_sudoers(sp->asker_name) == -1 && !sp->logging_option) {
        printf("%s is not in the sudoers file.\n", sp->asker_name);
        return 84;
    }
    if (sp->tries - 1 > 0)
        printf("Incorrect try : %d\n", sp->tries - 1);
    return 0;
}
