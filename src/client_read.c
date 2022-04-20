/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** client_read
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

int select_read(client_t *client, enum socket_type type)
{
    static fd_set readfds;
    struct timeval tv = {2, 0};
    int selected;
    int socket = get_client_socket(client, type);

    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);
    selected = select(socket + 1, &readfds, NULL, NULL, &tv);
    if (selected == -1) {
        perror("Can't read from client, select failed");
        return -1;
    }
    if (selected == 0) {
        return 0;
    }
    return 1;
}

ssize_t read_client(client_t *client, enum socket_type type, char *buf,
    size_t size)
{
    ssize_t n;
    int socket = get_client_socket(client, type);

    if (socket == -1) {
        dprintf(2, "Trying to write to a disconnected client\n");
        return 0;
    }
    n = select_read(client, type);
    if (n > 0)
        n = read(socket, buf, size);
    if (n == 0) {
        disconnect_client(client);
        return 0;
    }
    if (n < 0) {
        perror("Can't read from client");
        return -1;
    }
    return n;
}

static ssize_t readln_char(client_t *client, enum socket_type type, size_t *i,
    char *buff)
{
    ssize_t s;
    if (client->readbuf_pos < client->readbuf_size) {
        buff[(*i)++] = client->readbuf[client->readbuf_pos];
        client->readbuf_pos++;
    }
    if (client->readbuf[client->readbuf_pos] == '\n') {
        buff[(*i)++] = client->readbuf[client->readbuf_pos];
        client->readbuf_pos++;
        return *i;
    }
    if (client->readbuf_pos == client->readbuf_size) {
        s = read_client(client, type, client->readbuf, READBUF_SIZE);
        if (s <= 0)
            return s;
        client->readbuf[s] = '\0';
        client->readbuf_pos = 0;
        client->readbuf_size = s;
    }
    return -2;
}

int readln_client(client_t *client, enum socket_type type, char *buff,
    size_t size)
{
    ssize_t s;
    size_t i = 0;

    memset(buff, '\0', size);
    while (1) {
        if (i > size) {
            return size;
        }
        s = readln_char(client, type, &i, buff);
        if (s != -2) {
            return s;
        }
    }
    disconnect_client(client);
    return 0;
}
