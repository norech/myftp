/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** cmd
*/
#include "myftp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct cmd_s {
    char *name;
    void (*cmd)(client_t *client, int ac, char *av[]);
};

const struct cmd_s CMDS[] = {
    { "PASV", cmd_pasv },
    { "USER", cmd_user },
    { "PASS", cmd_pass },
    { "QUIT", cmd_quit },
    { "EXIT", cmd_quit },
    { "PWD", cmd_pwd },
    { "NOOP", cmd_noop }
};

void cmd(client_t *client, int ac, char *av[])
{
    long unsigned int i = 0;

    if (ac == 0)
        return;
    printf("[Client %d] Client sent command: [", client->ctrl_socket);
    for (int j = 0; j < ac; j++) {
        printf("'%s'", av[j]);
        printf(j != ac - 1 ? " " : "");
    }
    printf("]\n");
    for (i = 0; i < sizeof(CMDS) / sizeof(struct cmd_s); i++) {
        if (strcasecmp(CMDS[i].name, av[0]) == 0) {
            return CMDS[i].cmd(client, ac, av);
        }
    }
    send_ctrl_reply(client, ERR_CMD_UNKNOWN);
}

void run_cmd(client_t *client, char *param)
{
    char *token;
    char *cpy = strdup(param);
    int ac = 0;
    char *av[2048];

    for (token = strtok(cpy, " \t"); ac < 2048; token = strtok(NULL, " \t")) {
        if (token == NULL)
            break;
        av[ac] = token;
        ac++;
    }
    cmd(client, ac, av);
    free(cpy);
}
