/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** cmd_pasv
*/

#include <alloca.h>
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

void cmd_port(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    char *end = NULL;

    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    if (ac != 2)
        return (void)send_ctrl_reply(client, ERR_CMD_BAD_SEQ);
    if (parse_ftp_hostport(av[1], &client->active_host, &end) == -1
        || *end != '\0')
        return (void)send_ctrl_reply(client, ERR_CMD_UNKNOWN);
    client->type = C_ACTIVE;
    send_ctrl_reply(client, CMD_OK);
}
