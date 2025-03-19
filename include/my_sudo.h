/*
** EPITECH PROJECT, 2025
** include/my
** File description:
** include file for the project my_sudo
*/

#ifndef MY_SUDO_H_
    #define MY_SUDO_H_

    #include <stdio.h>
    #include <fcntl.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <grp.h>

typedef struct sudo_s {
    int as_user_option;
    char *name;
    char *asker_name;
    char *group;
    int env_preserve;
    char *shell;
    int shell_option;
    int tries;
    char *shell_command;
    int argc;
    char **argv;
    char **env;
    int start_exec;
    int uid_to_be;
    int should_quit;
    char **groups;
    int logging_option;
} sudo_t;

typedef struct env_s {
    int *default_values;
    char **to_keep;
    int **to_keep_values;
} env_t;

typedef struct passwd_s {
    char *user;
    char *pass;
    int uid;
    int gid;
    char *comment;
    char *home;
    char *shell;
} passwd_t;

/*
 * Main
 */
int is_correct_password(sudo_t *sudo_params);
int verifiy_sudo(sudo_t *sudo_params);

/*
 * String utils
 */
void remove_newline(char *str);
char *get_line_start_with(char *file_path, char *start_with);
char *init_empty_string(int malloc_size);

/*
 * Security utils
 */
int check_password(const char *password, const char *hashed_password,
    sudo_t *sudo_param);
char *ask_password(sudo_t *sudo_param);

/*
 * Shell utils
 */
int set_shell(sudo_t *sudo_params);
char **get_shell_command(sudo_t *sudo_params);
void free_shell_command(char **shell_c);

/*
 * Env utils
 */
char **create_env_default(void);
void set_env_sudo_defaults(sudo_t *sudo_params);
void set_env_sudo(sudo_t *sudo_params, char *key, char *value);

/*
 * List utils
 */
int show_list(sudo_t *sp);

/*
 * Sudo utils
 */
char *get_hash_from_name(char *username);
char *get_salt_from_hash(const char *hash);
int get_uid(char *id);
int get_gid(char *id);
passwd_t *get_passwd(char *id);
void free_passwd(passwd_t *p);
char *get_command_path(char *command);
int my_getpass(char **buffer, unsigned long *buffer_size);

/*
 * Parser utils
 */
sudo_t *get_sudo_params(int argc, char *argv[]);
char *get_asker_name(int uid);
void show_parser(sudo_t *sudo_params);

/*
 * Verification utils
 */
int is_user_valid(sudo_t *sudo_params);
int verif_sudo_and_check_password(sudo_t *sp);
void set_all_uid_and_gid(sudo_t *sp);


/*
 * Sudo id
 */
int get_gid_in_passwd(char *id);

/*
 * Sudo sudoers
 */
int check_id_in_sudoers(char *id);
int is_same_group(char *id, char **groups, char *buff, int i);
int is_same_person(char *id, char *buff);
int check_group(char *id, char *buff);

/*
 * Sudo groups functions
 */
char **get_all_groups(char *id);
void print_groups(char **groups);
int check_group_in_sudoers(char *id);
void set_all_groups(char *id);

#endif
