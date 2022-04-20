/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** util_crlf
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

bool strip_crlf(char *buff, size_t len)
{
    if (len == 0)
        len = strlen(buff);
    if (buff[len - 1] != '\n')
        return false;
    if (buff[len - 2] != '\r')
        return false;
    buff[len - 1] = '\0';
    buff[len - 2] = '\0';
    return true;
}
