/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** client_disconnect
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

void disconnect_client_data(client_t *client)
{
    bool disconnected = false;
    if (client->data_socket != -1) {
        FD_CLR(client->data_socket, &client->server->readfds);
        close(client->data_socket);
        client->data_socket = -1;
        disconnected = true;
    }
    if (client->srv_data_socket != -1) {
        FD_CLR(client->srv_data_socket, &client->server->readfds);
        close(client->srv_data_socket);
        client->srv_data_socket = -1;
        disconnected = true;
    }
    if (disconnected) {
        dprintf(2, "Client %d data disconnected\n", client->ctrl_socket);
    }
}

void disconnect_client(client_t *client)
{
    disconnect_client_data(client);
    if (client->ctrl_socket != -1) {
        send_ctrl_reply(client, SERVICE_LOGOUT);
        FD_CLR(client->ctrl_socket, &client->server->readfds);
        close(client->ctrl_socket);
        client->ctrl_socket = -1;
    }
    dprintf(2, "Client disconnected\n");
}
