/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** util_path
*/
#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "myftp.h"

char *resolve_multiarg_path(char *out, client_t *client, int arrlen,
    char **arr)
{
    char old_pwd[PATH_MAX];
    char path[PATH_MAX];
    size_t len = arrlen - 1;

    for (int i = 0; i < arrlen; i++)
        len += strlen(arr[i]);
    if (len > PATH_MAX)
        return NULL;
    memcpy(old_pwd, client->pwd, PATH_MAX);
    strcpy(path, arr[0]);
    for (int i = 1; i < arrlen; i++) {
        strcat(path, " ");
        strcat(path, arr[i]);
    }
    return resolve_path(out, client->server->pwd, old_pwd, path);
}
