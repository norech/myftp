/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** util_ip
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

int parse_ftp_host_part(char *hostport, struct sockaddr_in *addr, char **end,
    int *i)
{
    char ip[INET_ADDRSTRLEN];
    char *start = hostport;
    long number;

    memset(ip, 0, INET_ADDRSTRLEN);
    for (*i = 0; *i < 4; (*i)++) {
        number = strtol(start, end, 10);
        if (start == *end || number < 0 || number > 255)
            return -1;
        strncat(ip, start, *end - start);
        if (*i != 3)
            strcat(ip, ".");
        start = *end + 1;
    }
    if (inet_aton(ip, &addr->sin_addr) == 0)
        return -1;
    return (**end != ',') ? -1 : 0;
}

int parse_ftp_hostport(char *hostport, struct sockaddr_in *addr, char **end)
{
    char *start = hostport;
    unsigned char number;
    int i;
    int j;
    *end = start;
    if (parse_ftp_host_part(start, addr, end, &i) == -1)
        return -1;
    start = *end + 1;
    addr->sin_port = 0;
    for (j = 0; j < 2; j++) {
        number = strtol(start, end, 10);
        if (start == *end)
            return -1;
        addr->sin_port |= (number << 8 * j);
        start = *end + 1;
    }
    return i == 4 && j == 2 ? 0 : -1;
}
