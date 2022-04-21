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

bool is_path_in_base(char *base, char *path)
{
    char base_cpy[PATH_MAX];
    char path_cpy[PATH_MAX];

    strncpy(base_cpy, base, PATH_MAX - 1);
    if (base_cpy[strlen(base_cpy) - 1] != '/')
        strcat(base_cpy, "/");
    strncpy(path_cpy, path, PATH_MAX - 1);
    if (path_cpy[strlen(path_cpy) - 1] != '/')
        strcat(path_cpy, "/");
    return (strncmp(base_cpy, path_cpy, strlen(base_cpy)) == 0);

}

char *root_to_base(char *out, char *base, char *path)
{
    size_t base_len = strlen(base);
    if (path[0] != '/')
        return false;
    if (strcmp(path, "/") == 0) {
        strcpy(out, base);
        return out;
    }
    if (base[base_len - 1] == '/') {
        if(base_len - 1 + strlen(path) >= PATH_MAX)
            return NULL;
        strcpy(out, base);
        strcat(out, path + 1);
    } else {
        if(base_len + strlen(path) >= PATH_MAX)
            return NULL;
        strcpy(out, base);
        strcat(out, path);
    }
    return out;
}

char *base_to_root(char *out, char *base, char *path)
{
    if (!is_path_in_base(base, path))
        return NULL;
    if (strlen(path) < strlen(base))
        return NULL;
    if (base[strlen(base) - 1] == '/')
        strcpy(out, path + strlen(base) - 1);
    else
        strcpy(out, path + strlen(base));
    if (out[0] == '\0')
        strcpy(out, "/");
    return out;
}

char *resolve_path(char *out, char *root, char *abs_pwd, char *path)
{
    char abs[PATH_MAX];
    char *tmp;

    tmp = base_to_root(out, root, abs_pwd);
    if (tmp == NULL)
        return NULL;
    printf("aa %s\n", tmp);
    memcpy(abs, tmp, PATH_MAX);
    tmp = resolve_relatives(out, "/", path);
    if (tmp == NULL)
        return NULL;
    printf("bb %s\n", tmp);
    memcpy(abs, tmp, PATH_MAX);
    tmp = root_to_base(out, root, abs);
    if (tmp == NULL)
        return NULL;
    printf("cc %s\n", tmp);
    memcpy(abs, tmp, PATH_MAX);
    if (!is_path_in_base(root, abs))
        return NULL;
    printf("dd %s\n", tmp);
    return tmp;
}
