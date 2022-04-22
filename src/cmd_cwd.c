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
#include "util_ip.h"
#include "util_error.h"
#include "myftp.h"

void cmd_cwd(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    char old_pwd[PATH_MAX];

    if (ac != 2)
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_ACCESS);
    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    memcpy(old_pwd, client->pwd, PATH_MAX);
    if (resolve_path(client->pwd, client->server->pwd,
        old_pwd, av[1]) == NULL) {
        memcpy(client->pwd, old_pwd, PATH_MAX);
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_STORAGE);
    }
    if (access(client->pwd, X_OK) != 0) {
        memcpy(client->pwd, old_pwd, PATH_MAX);
        return (void)send_ctrl_reply(client, ERR_ACTION_NOT_TAKEN_ACCESS);
    }
    if (strcmp(av[0], "cdup") == 0)
        return (void)send_ctrl_reply(client, CMD_OK);
    send_ctrl_reply(client, SUCC_FILE_ACTION);
}
