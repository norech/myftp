/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** cmd_user
*/

#include <alloca.h>
#include <limits.h>
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

bool is_valid_anonymous(client_t *client, int ac, char *av[])
{
    int len;

    if (strcmp(client->user, "Anonymous") == 0) {
        if (ac == 2 || strcmp(av[1], "") == 0)
            return true;
    }
    if (strcmp(client->user, "anonymous") == 0) {
        len = strlen(av[1]);
        if (len > 12 && strcmp(av[1] + len - 12, "@example.com") == 0)
            return true;
    }
    return false;
}

void cmd_pass(client_t *client, int ac, char *av[])
{
    if (ac != 1 && ac != 2)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    if (client->logged_in)
        return (void)send_ctrl_reply(client, SUCC_LOGGED_IN);
    if (!is_valid_anonymous(client, ac, av))
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    client->logged_in = true;
    send_ctrl_reply(client, SUCC_LOGGED_IN);
}
