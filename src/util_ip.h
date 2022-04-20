/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** util_ip
*/

#pragma once

void print_ip(struct sockaddr_in *addr);

int get_addr(char *host, struct sockaddr_in *addr);
