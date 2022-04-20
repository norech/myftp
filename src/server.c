/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** server
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

void server_loop(server_t *server)
{
    fd_set readfds = server->readfds;
    struct timeval tv = {2, 0};
    int select_ret = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

    if (select_ret == -1)
        fatal_errno("Select failed");
    else if (select_ret == 0)
        return;
    for (int fd = 0; fd < FD_SETSIZE; fd++) {
        if (!FD_ISSET(fd, &readfds))
            continue;
        if (FD_ISSET(fd, &server->datafds)) {
            handle_data_socket_fd(fd, server);
        }
        if (FD_ISSET(fd, &server->ctrlfds)) {
            handle_ctrl_socket_fd(fd, server);
        }
    }
}

void init_server(server_t *server, int port)
{
    struct sockaddr_in addr;

    server->clients = calloc(FD_SETSIZE, sizeof(client_t));
    server->ctrl_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server->ctrl_socket, (struct sockaddr *)&addr,
    sizeof(addr)) < 0)
        fatal_errno("Can't open socket");
    if (listen(server->ctrl_socket, 5) < 0)
        fatal_errno("Can't listen to port");
    FD_ZERO(&server->readfds);
    FD_ZERO(&server->ctrlfds);
    FD_ZERO(&server->datafds);
    FD_SET(server->ctrl_socket, &server->readfds);
    FD_SET(server->ctrl_socket, &server->ctrlfds);
}

int main(int ac, char *av[])
{
    struct server_s server;

    if (ac != 3)
        fatal("Usage: %s port path", av[0]);
    if (access(av[2], X_OK) != 0)
        fatal("Can't access '%s'", av[2]);
    server.pwd = av[2];
    init_server(&server, atoi(av[1]));
    while (1)
        server_loop(&server);
    close(server.ctrl_socket);
    return 0;
}
