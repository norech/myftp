/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** cmd_user
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

void cmd_help(client_t *client, int ac UNUSED, char *av[] UNUSED)
{
    printf_client(client, S_CTRL,
        "214-The following commands are recognized."CRLF);
    list_commands(client);
    printf_client(client, S_CTRL, "214 Help OK."CRLF);
}
