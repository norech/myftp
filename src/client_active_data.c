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

static void fill_addr(struct sockaddr_in *addr, client_t *client,
    struct sockaddr_in *active_addr)
{
    memset(addr, 0, sizeof(*addr));
    active_addr->sin_family = AF_INET;
    active_addr->sin_port = ntohs(active_addr->sin_port);
    memcpy(addr, active_addr, sizeof(struct sockaddr_in));
    memcpy(&client->active_host, active_addr, sizeof(struct sockaddr_in));
}

int connect_to_client_data(client_t *client)
{
    struct sockaddr_in addr;
    if (client->data_socket != -1)
        return -1;
    fill_addr(&addr, client, &client->active_host);
    client->data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client->data_socket == -1)
        return -1;
    if (connect(client->data_socket, (struct sockaddr *)&addr,
        sizeof(addr)) == -1) {
        close(client->data_socket);
        client->data_socket = -1;
        return -1;
    }
    FD_SET(client->data_socket, &client->server->readfds);
    FD_SET(client->data_socket, &client->server->datafds);
    return 0;
}
