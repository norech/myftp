/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** util_error
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

void fatal(char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vdprintf(2, msg, ap);
    va_end(ap);
    dprintf(2, "\n");
    exit(84);
}

void fatal_errno(char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vdprintf(2, msg, ap);
    va_end(ap);
    dprintf(2, ": %s\n", strerror(errno));
    exit(84);
}
