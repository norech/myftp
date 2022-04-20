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

void cmd_pwd(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    send_ctrl_reply(client, SUCC_PATHNAME, client->pwd);
}
