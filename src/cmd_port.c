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

static void fill_addr(struct sockaddr_in *addr, client_t *client, int port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = client->ctrl_addr.sin_addr.s_addr;
    addr->sin_port = htons(port);
}

static void finish_port_init(client_t *client)
{
    client->type = C_ACTIVE;
    send_ctrl_reply(client, TRANSFER_DATA_OPEN);
    FD_SET(client->data_socket, &client->server->readfds);
    FD_SET(client->data_socket, &client->server->datafds);
}

void cmd_port(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    struct sockaddr_in addr;
    char *end;
    long port = strtol(av[1], &end, 10);
    if (av[1] == end || port < 0 || port > USHRT_MAX)
        return (void)send_ctrl_reply(client, ERR_CMD_UNKNOWN);
    if (client->data_socket != -1)
        return (void)send_ctrl_reply(client, TRANSFER_DATA_ALREADY_OPEN);
    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    fill_addr(&addr, client, port);
    client->data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client->data_socket, (struct sockaddr *)&addr,
        sizeof(addr)) == -1) {
        close(client->data_socket);
        client->data_socket = -1;
        return (void)send_ctrl_reply(client, ERR_CANT_OPEN_DATACON);
    }
    finish_port_init(client);
}
