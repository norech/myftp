/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** client_socket
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

client_t *get_client_ctrl(int fd, server_t *server)
{
    if (!FD_ISSET(fd, &server->readfds))
        return NULL;
    if (server->clients[fd].ctrl_socket != fd)
        return NULL;
    return &server->clients[fd];
}

int get_client_socket(client_t *client, enum socket_type type)
{
    switch (type) {
        case S_CTRL:
            return client->ctrl_socket;
        case S_DATA:
            return client->data_socket;
        default:
            return -1;
    }
}

enum socket_type get_socket_type(int fd, client_t *client)
{
    if (fd == client->ctrl_socket)
        return S_CTRL;
    if (fd == client->data_socket)
        return S_DATA;
    return -1;
}

bool is_client_connected(client_t *client, enum socket_type type)
{
    if (client == NULL)
        return false;
    return get_client_socket(client, type) != -1;
}
