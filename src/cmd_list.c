/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** cmd_user
*/

#include <alloca.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <values.h>
#include <dirent.h>
#include <time.h>
#include <libgen.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include "util_ip.h"
#include "util_error.h"
#include "myftp.h"

static int resolve_dir(char *out, client_t *client, int ac, char **av)
{
    if (ac == 1) {
        memcpy(out, client->pwd, PATH_MAX);
        return 0;
    }
    if (resolve_path(out, client->server->pwd,
        client->pwd, av[1]) == NULL) {
            send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_STORAGE);
        return -1;
    }
    if (access(client->pwd, X_OK) != 0) {
        send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_ACCESS);
        return -1;
    }
    return 0;
}

static void print_file(client_t *client, struct stat *st, char *name)
{
    char time[13] = {0};
    strncpy(time, ctime(&(st->st_mtime)) + 4, 12);
    printf_client(client, S_DATA, "%c%c%c%c%c%c%c%c%c%c",
    S_ISDIR(st->st_mode) ? 'd' : '-', (st->st_mode & S_IRUSR) ? 'r' : '-',
    (st->st_mode & S_IWUSR) ? 'w' : '-', (st->st_mode & S_IXUSR) ? 'x' : '-',
    (st->st_mode & S_IRGRP) ? 'r' : '-', (st->st_mode & S_IWGRP) ? 'w' : '-',
    (st->st_mode & S_IXGRP) ? 'x' : '-', (st->st_mode & S_IROTH) ? 'r' : '-',
    (st->st_mode & S_IWOTH) ? 'w' : '-', (st->st_mode & S_IXOTH) ? 'x' : '-');
    printf_client(client, S_DATA, " %4d %-12s %-12s %12ld %s %s"CRLF,
        (int)st->st_nlink, getpwuid(st->st_uid)->pw_name,
        getgrgid(st->st_gid)->gr_name, st->st_size, time, name);
}

static void list_file(client_t *client, char *path, char *name)
{
    char full_path[PATH_MAX] = {0};
    struct stat st = {0};

    if (resolve_path(full_path, client->server->pwd,
        path, name) == NULL)
        return;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        return;
    if (stat(full_path, &st) != 0)
        return;
    print_file(client, &st, name);
}

static void list_files(client_t *client, char *path, struct stat *st)
{
    DIR *d;
    struct dirent *dir;
    char base[PATH_MAX] = {0};
    char dirn[PATH_MAX] = {0};
    memcpy(base, path, PATH_MAX - 1);
    memcpy(dirn, path, PATH_MAX - 1);
    if (stat(path, st) != 0)
        return;
    send_ctrl_reply(client, FILE_STATUS_OKAY);
    if (!S_ISDIR(st->st_mode)) {
        return list_file(client, dirname(dirn), basename(base));
    }
    d = opendir(path);
    if (d == NULL)
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_ACCESS);
    while ((dir = readdir(d)) != NULL) {
        list_file(client, path, dir->d_name);
    }
    closedir(d);
}

void cmd_list(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    char dir[PATH_MAX] = {0};
    struct stat st = {0};

    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    if (is_client_connected(client, S_DATA))
        return (void)send_ctrl_reply(client, ERR_CANT_OPEN_DATACON);
    if (resolve_dir(dir, client, ac, av) == -1)
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_STORAGE);
    if (fork() == 0) {
        if (await_client_connection(client) == -1) {
            disconnect_client_data(client);
            return (void)send_ctrl_reply(client, ERR_CANT_OPEN_DATACON);
        }
        list_files(client, dir, &st);
        send_ctrl_reply(client, SUCC_DATACON_CLOSE);
        disconnect_client_data(client);
        exit(0);
    }
    disconnect_client_data(client);
}
