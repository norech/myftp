/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** passive
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

client_t *get_client_data(int fd, server_t *server)
{
    if (!FD_ISSET(fd, &server->readfds))
        return NULL;
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (server->clients[i].data_socket == fd
            && server->clients[i].server == server)
            return &server->clients[i];
    }
    return NULL;
}

client_t *get_client_srv_data(int fd, server_t *server)
{
    if (!FD_ISSET(fd, &server->readfds))
        return NULL;
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (server->clients[i].srv_data_socket == fd
            && server->clients[i].server == server)
            return &server->clients[i];
    }
    return NULL;
}

client_t *accept_client_data(client_t *client_s, server_t *server UNUSED)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int fd = accept(client_s->srv_data_socket,
        (struct sockaddr *)&client, &len);
    if (fd < 0) {
        perror("Can't accept data connection");
        return NULL;
    }
    client_s->data_addr = client;
    client_s->data_socket = fd;
    printf("Accepting client data for client %d\n", client_s->ctrl_socket);
    return client_s;
}

int handle_data_socket_fd(int fd, server_t *server)
{
    client_t *client = get_client_srv_data(fd, server);
    char buf[1024];
    ssize_t n;

    if (client != NULL) {
        accept_client_data(client, server);
        return 0;
    }
    client = get_client_data(fd, server);
    if (!is_client_connected(client, S_DATA))
        return 0;
    n = read_client(client, S_DATA, buf, sizeof(buf));
    while (n > 0) {
        write(1, buf, n);
        printf_client(client, S_DATA, "hello world"CRLF);
    }
    return 0;
}

int await_client_passive_connection(client_t *client)
{
    fd_set set;
    int s;
    struct timeval tv = {5, 0};
    FD_ZERO(&set);
    FD_SET(client->srv_data_socket, &set);
    s = select(client->srv_data_socket + 1, &set, NULL, NULL, &tv);
    if (s <= 0) {
        return -1;
    }
    accept_client_data(client, client->server);
    return 0;
}
