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

void cmd_pass(client_t *client, int ac, char *av[])
{
    if (ac != 1 && ac != 2)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    if (client->logged_in)
        return (void)send_ctrl_reply(client, SUCC_LOGGED_IN);
    if (strcmp(client->user, "Anonymous") != 0
        || (ac == 2 && strcmp(av[1], "") != 0))
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    client->logged_in = true;
    send_ctrl_reply(client, SUCC_LOGGED_IN);
}
