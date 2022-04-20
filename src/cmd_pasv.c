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

void cmd_pasv(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    struct sockaddr_in addr;
    uint32_t len = sizeof(addr);
    if (client->data_socket != -1)
        return;
    if (!client->logged_in)
        return (void)send_ctrl_reply(client, ERR_NOT_LOGGED_IN);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr.s_addr = INADDR_ANY;
    client->srv_data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(client->srv_data_socket, (struct sockaddr *)&addr,
        sizeof(addr)) < 0 || listen(client->srv_data_socket, 5) < 0) {
        close(client->srv_data_socket);
        return (void)send_ctrl_reply(client, ERR_CANT_OPEN_DATACON);
    }
    getsockname(client->srv_data_socket, (struct sockaddr *)&addr,
        &len);
    send_ctrl_reply(client, SUCC_PASV_ENTER, addr.sin_port / 256,
        addr.sin_port % 256);
}
