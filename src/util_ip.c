/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** util_ip
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

void print_ip(struct sockaddr_in *addr)
{
    unsigned int ip = addr->sin_addr.s_addr;
    unsigned char bytes[4];
    ip = ntohl(ip);
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    printf("%d.%d.%d.%d:%d\n", bytes[3], bytes[2], bytes[1], bytes[0],
        ntohs(addr->sin_port));
}

int get_hostname(char *out, char *host)
{
    char *port_separator = strchr(host, ':');

    memset(out, 0, strlen(host));
    if (port_separator)
        strncpy(out, host, port_separator - host);
    else
        strcpy(out, host);
    return 0;
}

int get_port(char *host)
{
    char *port_separator = strchr(host, ':');
    if (port_separator && (port_separator[1] < '0' || port_separator[1] > '9'))
        return -1;
    return port_separator == NULL ? 21 : atoi(port_separator + 1);
}

int get_addr(char *host, struct sockaddr_in *addr)
{
    struct hostent *he;
    struct in_addr **addr_list;
    char *hostname = alloca(strlen(host) + 1);

    get_hostname(hostname, host);
    if ((he = gethostbyname(hostname)) == NULL) {
        printf("%s: unknown host\n", hostname);
        herror("gethostbyname");
        return -1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; i++) {
        addr->sin_family = AF_INET;
        addr->sin_addr = *addr_list[i];
        addr->sin_port = htons(get_port(host));
        return 1;
    }
    return 0;
}
