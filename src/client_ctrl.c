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

void prepare_client(int fd, struct sockaddr_in client, client_t *client_s,
    server_t *server)
{
    memset(&server->clients[fd], '\0', sizeof(client_t));
    client_s->ctrl_socket = fd;
    client_s->data_socket = -1;
    client_s->srv_data_socket = -1;
    client_s->ctrl_addr = client;
    client_s->state = STATE_TEXT;
    client_s->server = server;
    strncpy(client_s->pwd, server->pwd, 255);
    send_ctrl_reply(client_s, SERVICE_READY);
}

client_t *accept_client_ctrl(server_t *server)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    client_t *client_s;

    int fd = accept(server->ctrl_socket, (struct sockaddr *)&client, &len);
    if (fd < 0) {
        perror("Can't accept control connection");
        return NULL;
    }
    FD_SET(fd, &server->readfds);
    FD_SET(fd, &server->ctrlfds);
    client_s = &server->clients[fd];
    prepare_client(fd, client, client_s, server);
    return client_s;
}

int handle_ctrl_client(client_t *client)
{
    char buf[1024];
    ssize_t n;

    if ((n = readln_client(client, S_CTRL, buf, sizeof(buf) - 1)) > 0) {
        if (!strip_crlf(buf, n)) {
            dprintf(2, "Client sent a too long line!");
            return -1;
        }
        run_cmd(client, buf);
    }
    if (n < 0) {
        perror("Can't discuss with client");
        disconnect_client(client);
        return -1;
    }
    return 0;
}

int handle_ctrl_socket_fd(int fd, server_t *server)
{
    client_t *client;

    if (fd == server->ctrl_socket) {
        client = accept_client_ctrl(server);
        return 0;
    }
    client = get_client_ctrl(fd, server);
    if (!is_client_connected(client, S_CTRL))
        return 0;
    return handle_ctrl_client(client);
}
