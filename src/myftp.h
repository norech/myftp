/*
** EPITECH PROJECT, 2022
** bsmyftp
** File description:
** myftp
*/

#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <stdbool.h>

#define UNUSED __attribute__((unused))
#define READBUF_SIZE 1024

enum state {
    STATE_TEXT,
    STATE_BINARY
};

enum socket_type {
    S_CTRL,
    S_DATA
};

enum connection_type {
    C_UNDEFINED = 0,
    C_PASSIVE,
    C_ACTIVE
};

struct client_s {
    int ctrl_socket;
    int data_socket;
    int srv_data_socket;
    enum state state;
    enum connection_type type;
    struct sockaddr_in ctrl_addr;
    struct sockaddr_in data_addr;
    struct server_s *server;
    char user[256];
    char pwd[4096];
    bool logged_in;
    char readbuf[READBUF_SIZE];
    size_t readbuf_size;
    size_t readbuf_pos;
};

struct server_s {
    struct client_s *clients;
    fd_set readfds;
    fd_set ctrlfds;
    fd_set datafds;
    char *pwd;
    int ctrl_socket;
};

typedef struct client_s client_t;
typedef struct server_s server_t;

#define CRLF "\r\n"
#define SP " "

size_t send_ctrl_reply(client_t *client, int code, ...);

void cmd_pasv(client_t *client, int ac, char *av[]);
void cmd_user(client_t *client, int ac, char *av[]);
void cmd_pass(client_t *client, int ac, char *av[]);
void cmd_quit(client_t *client, int ac, char *av[]);
void cmd_pwd(client_t *client, int ac, char *av[]);
void cmd_noop(client_t *client, int ac, char *av[]);


client_t *get_client_ctrl(int fd, server_t *server);

bool is_client_connected(client_t *client, enum socket_type type);

void disconnect_client(client_t *client);

enum socket_type get_socket_type(int fd, client_t *client);

int get_client_socket(client_t *client, enum socket_type type);

client_t *accept_client_ctrl(server_t *server);

ssize_t write_client(client_t *client, enum socket_type type, char *msg,
    size_t size);

ssize_t writeln_client(client_t *client, enum socket_type type, char *msg);

ssize_t printf_client(client_t *client, enum socket_type type, char *fmt, ...);

ssize_t read_client(client_t *client, enum socket_type type, char *buf,
    size_t size);

bool strip_crlf(char *buff, size_t len);

int readln_client(client_t *client, enum socket_type type, char *buff,
    size_t size);

void run_cmd(client_t *client, char *param);

int handle_data_socket_fd(int fd, server_t *server);

int handle_ctrl_socket_fd(int fd, server_t *server);

struct response {
    int code;
    char *msg;
};

enum response_code {
    CMD_OK = 200,
    ERR_CMD_UNKNOWN = 500,
    ERR_CMD_NOT_IMPL = 502,
    ERR_CMD_BAD_SEQ = 503,
    ERR_CMD_PARAM_NOT_IMPL = 504,
    SUCC_PASV_ENTER = 227,
    RESTART_MARKER_REPLY = 110,
    STATUS_SYSTEM = 211,
    STATUS_DIR = 212,
    STATUS_FILE = 213,
    STATUS_HELP = 214,
    SERVICE_READY_LATER = 120,
    SERVICE_READY = 220,
    SERVICE_LOGOUT = 221,
    SERVICE_FORCED_LOGOUT = 421,
    TRANSFER_DATA_ALREADY_OPEN = 125,
    TRANSFER_DATA_OPEN = 225,
    ERR_CANT_OPEN_DATACON = 425,
    SUCC_DATACON_CLOSE = 226,
    ERR_CON_CLOSED = 426,
    SUCC_LOGGED_IN = 230,
    ERR_NOT_LOGGED_IN = 530,
    SUCC_NEED_PASSWORD = 331,
    NEED_ACCOUNT_FOR_LOGIN = 332,
    NEED_ACCOUNT_FOR_STORING_FILES = 532,
    FILE_STATUS_OKAY = 150,
    ERR_USER_NOT_LOCAL = 151,
    ERR_USER_UNKNOWN = 152,
    SUCC_FILE_ACTION = 250,
    PEND_FILE_ACTION = 350,
    ERR_ACTION_NOT_TAKEN_ACCESS = 550,
    ERR_ACTION_ABORTED_INTERNAL = 451,
    ERR_ACTION_ABORTED_PAGE_TYPE = 551,
    ERR_ACTION_NOT_TAKEN_STORAGE = 452,
    ERR_ACTION_ABORTED_STORAGE = 552,
    ERR_INVALID_FILE_NAME = 553,
    SUCC_PATHNAME = 257,
    MAIL_INPUT = 354
};

bool strip_crlf(char *buff, size_t len);
