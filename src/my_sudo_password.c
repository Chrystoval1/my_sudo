/*
** EPITECH PROJECT, 2025
** src/my_sudo_hash
** File description:
** This file contains functions for checking the hash function
*/

#include "../include/my_sudo.h"
#include <stdio.h>
#include <termios.h>

char *get_salt_from_hash(const char *hash)
{
    char *salt;
    int salt_len = 0;
    int i_dollar = 0;

    while (hash[salt_len] != '\0') {
        if (hash[salt_len] == '$' && i_dollar == 3)
            break;
        if (hash[salt_len] == '$')
            i_dollar++;
        salt_len++;
    }
    salt = malloc(sizeof(char) * (salt_len + 1));
    for (int i = 0; i < salt_len; i++) {
        salt[i] = hash[i];
    }
    salt[salt_len] = '\0';
    return salt;
}

char *get_hash_from_name(char *username)
{
    char *buffer = get_line_start_with("/etc/shadow", username);
    int i = 0;
    int j = 0;
    int len = strlen(buffer);
    char *mem = malloc(sizeof(char) * strlen(buffer));

    if (buffer == NULL || username == NULL)
        return NULL;
    for (int k = 0; k < len; k++)
        mem[k] = '\0';
    while (buffer[i] != ':')
        i++;
    i++;
    while (buffer[i] != ':') {
        mem[j] = buffer[i];
        j++;
        i++;
    }
    free(buffer);
    return mem;
}

int check_password(const char *password, const char *hashed_password,
    sudo_t *sudo_params)
{
    char *hashed;
    char *salt = get_salt_from_hash(hashed_password);

    if (strcmp(sudo_params->name, sudo_params->asker_name) == 0)
        return 1;
    hashed = crypt(password, salt);
    if (strcmp(hashed, hashed_password) == 0) {
        free(salt);
        return 1;
    }
    free(salt);
    return 0;
}

char *ask_password(sudo_t *sudo_param)
{
    char *password = NULL;
    unsigned long password_len = 0;
    struct termios termios;

    if (strcmp(sudo_param->name, sudo_param->asker_name) != 0) {
        printf("[my_sudo] password for %s: ", sudo_param->asker_name);
        fflush(stdout);
        tcgetattr(STDIN_FILENO, &termios);
        termios.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
        getline(&password, &password_len, stdin);
        tcgetattr(STDIN_FILENO, &termios);
        termios.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
        remove_newline(password);
        write(1, "\n", 1);
        sudo_param->tries++;
    }
    return password;
}
