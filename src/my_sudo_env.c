/*
** EPITECH PROJECT, 2025
** src/sudo_env
** File description:
** file for environnement changes
*/

#include "../include/my_sudo.h"
#include <string.h>

static int is_in_default_names(char *var_name)
{
    char *default_names[16] = {"SHELL", "HOSTNAME", "LANGUAGE", "PWD",
        "LOGNAME", "HOME", "LANG", "LS_COLORS", "TERM", "USER",
        "DISPLAY", "SHLVL", "LC_ALL", "PATH", "MAIL", NULL};

    for (int j = 0; default_names[j] != NULL; j++) {
        if (strcmp(var_name, default_names[j]) == 0)
            return 1;
    }
    return 0;
}

static char *extract_var_name(char *env_str)
{
    char *equal_sign = strchr(env_str, '=');
    int name_len = strlen(env_str) - strlen(equal_sign);
    char *var_name = malloc(name_len + 1);

    if (!equal_sign) {
        free(var_name);
        return NULL;
    }
    strncpy(var_name, env_str, name_len);
    var_name[name_len] = '\0';
    return var_name;
}

static int get_amount_env_var(void)
{
    extern char **environ;
    int count = 0;
    char *tmp_var_name;

    for (int i = 0; environ[i]; i++) {
        tmp_var_name = extract_var_name(environ[i]);
        if (tmp_var_name != NULL && is_in_default_names(tmp_var_name)) {
            count++;
        }
        free(tmp_var_name);
    }
    return count;
}

static void copy_env_var(char **new_env)
{
    extern char **environ;
    int new_index = 0;
    char *tmp_var_name;

    for (int i = 0; environ[i]; i++) {
        tmp_var_name = extract_var_name(environ[i]);
        if (tmp_var_name != NULL && is_in_default_names(tmp_var_name)) {
            new_env[new_index] = strdup(environ[i]);
            new_index += 1;
        }
        free(tmp_var_name);
    }
    new_env[new_index] = NULL;
}

char **create_env_default(void)
{
    int count = get_amount_env_var();
    char **new_env = malloc(sizeof(char *) * (count + 1));

    copy_env_var(new_env);
    return new_env;
}

void set_env_sudo_defaults(sudo_t *sudo_params)
{
    passwd_t *p = get_passwd(sudo_params->name);

    set_env_sudo(sudo_params, "LOGNAME", p->user);
    set_env_sudo(sudo_params, "HOME", p->home);
    free_passwd(p);
}

void set_env_sudo(sudo_t *sudo_params, char *key, char *value)
{
    char **env = sudo_params->env;
    char *temp = NULL;
    char *eq_pos = NULL;
    int key_len = 0;

    for (int i = 0; env[i] != NULL; i++) {
        eq_pos = strchr(env[i], '=');
        key_len = eq_pos - env[i];
        if (strncmp(env[i], key, key_len) == 0 && key[key_len] == '\0') {
            asprintf(&temp, "%s=%s", key, value);
            free(env[i]);
            env[i] = temp;
            return;
        }
    }
}
