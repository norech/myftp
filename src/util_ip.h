/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** util_ip
*/

#pragma once

#include <netinet/in.h>

void print_ip(struct sockaddr_in *addr);

int get_addr(char *host, struct sockaddr_in *addr);

int parse_ftp_hostport(char *hostport, struct sockaddr_in *addr, char **end);
