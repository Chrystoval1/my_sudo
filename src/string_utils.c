/*
** EPITECH PROJECT, 2025
** src/string_utils
** File description:
** This file contains some functions related for strings
*/

#include "../include/my_sudo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Remove the \n for the last character and put it as \0
 */
void remove_newline(char *str)
{
    int len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

/*
 * Get the content of a line by typing only the start of the line.
 */
char *get_line_start_with(char *file_path, char *start_with)
{
    FILE *file = fopen(file_path, "r");
    size_t size_len = 512;
    char *line = malloc(sizeof(char) * size_len);
    int len_start = strlen(start_with);

    if (file == NULL || line == NULL) {
        if (file != NULL)
            fclose(file);
        free(line);
        return NULL;
    }
    while (fgets(line, size_len, file) != NULL) {
        if (strncmp(line, start_with, len_start) == 0) {
            fclose(file);
            return line;
        }
    }
    free(line);
    fclose(file);
    return NULL;
}

char *init_empty_string(int malloc_size)
{
    char *string = malloc(sizeof(char) * (malloc_size + 1));

    for (int i = 0; i < malloc_size; i++) {
        string[i] = '\0';
    }
    return string;
}
