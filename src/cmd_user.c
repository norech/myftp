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

void cmd_user(client_t *client, int ac, char *av[])
{
    client->logged_in = false;

    if (ac < 2)
        *client->user = '\0';
    else
        strncpy(client->user, av[1],
            (sizeof(client->user) / sizeof(char)) - 1);
    send_ctrl_reply(client, SUCC_NEED_PASSWORD);
}
