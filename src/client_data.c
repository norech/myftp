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

int await_client_connection(client_t *client)
{
    if (client->type == C_ACTIVE) {
        return connect_to_client_data(client);
    }
    if (client->type == C_PASSIVE) {
        return await_client_passive_connection(client);
    }
    return -1;
}
