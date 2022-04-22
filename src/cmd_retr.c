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
#include <fcntl.h>

static int resolve_dir(char *out, client_t *client, int ac, char **av)
{
    if (ac == 1) {
        memcpy(out, client->pwd, PATH_MAX);
        return 0;
    }
    if (resolve_multiarg_path(out, client, ac - 1, av + 1) == NULL) {
        send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_STORAGE);
        return -1;
    }
    if (access(client->pwd, X_OK) != 0) {
        send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_ACCESS);
        return -1;
    }
    return 0;
}

static void retrieve_file(client_t *client, char *file, unsigned char *buffer)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_STORAGE);
    send_ctrl_reply(client, FILE_STATUS_OKAY);
    for (int nb_read; true;) {
        nb_read = read(fd, buffer, 2048);
        if (nb_read == -1) {
            close(fd);
            return (void)send_ctrl_reply(client, ERR_ACTION_ABORTED_STORAGE);
        }
        if (nb_read == 0)
            break;
        if (write(client->data_socket, buffer, nb_read) == -1) {
            close(fd);
            return (void)send_ctrl_reply(client, ERR_ACTION_ABORTED_INTERNAL);
        }
    }
    close(fd);
    send_ctrl_reply(client, SUCC_DATACON_CLOSE);
}

void cmd_retr(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    char dir[PATH_MAX] = {0};
    unsigned char buffer[2048] = {0};

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
        retrieve_file(client, dir, buffer);
        disconnect_client_data(client);
        exit(0);
    }
    disconnect_client_data(client);
}
