/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** util_path_relative
*/
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>

static int resolve_token(char *resolved, char *token)
{
    char *tmp;

    if (strcmp(token, "..") == 0) {
        tmp = strrchr(resolved, '/');
        if (tmp)
            *tmp = '\0';
        if (tmp == resolved)
            strcpy(resolved, "/");
    }
    else if (strcmp(token, ".") == 0)
        return 0;
    else {
        if (strlen(resolved) + strlen(token) + 1 > PATH_MAX)
            return -1;
        if (strcmp(resolved, "/") != 0)
            strcat(resolved, "/");
        strcat(resolved, token);
    }
    return 0;
}

char *resolve_relatives(char *out, char *base, char *path)
{
    char *path_ptr = strdup(path);

    if (path[0] == '/')
        strcpy(out, "/");
    else
        strcpy(out, base);
    if (strlen(out) != 1 && out[strlen(out) - 1] == '/')
        out[strlen(out) - 1] = '\0';
    for (char *token = strtok(path_ptr, "/"); token;
        token = strtok(NULL, "/")) {
        if (resolve_token(out, token) == -1) {
            free(path_ptr);
            return NULL;
        }
    }
    free(path_ptr);
    return out;
}
