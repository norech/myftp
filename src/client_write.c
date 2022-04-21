/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** write_client
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

int select_write(client_t *client, enum socket_type type)
{
    static fd_set writefds;
    struct timeval tv = {2, 0};
    int selected;
    int socket = get_client_socket(client, type);

    FD_ZERO(&writefds);
    FD_SET(socket, &writefds);
    selected = select(socket + 1, NULL, &writefds, NULL, &tv);
    if (selected == -1) {
        perror("Can't write to client, select failed");
        return -1;
    }
    if (selected == 0) {
        dprintf(2, "Write failed, connection timed out\n");
        return 0;
    }
    return 1;
}

ssize_t write_client(client_t *client, enum socket_type type, char *msg,
    size_t size)
{
    ssize_t n;
    int fd = get_client_socket(client, type);

    if (fd == -1) {
        dprintf(2, "Trying to write to a disconnected client\n");
        return 0;
    }
    n = write(fd, msg, size);
    if (n < 0) {
        perror("Can't write to client");
        return -1;
    }
    return n;
}

ssize_t writeln_client(client_t *client, enum socket_type type, char *msg)
{
    char *msgln = alloca(sizeof(char) * (strlen(msg) + 2));

    strcpy(msgln, msg);
    strcat(msgln, CRLF);
    return write_client(client, type, msgln, strlen(msgln));
}

ssize_t printf_client(client_t *client, enum socket_type type, char *fmt, ...)
{
    static char msg[4096] = {0};
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, 4096, fmt, ap);
    va_end(ap);
    return write_client(client, type, msg, strlen(msg));
}
