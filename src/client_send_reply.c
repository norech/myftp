/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** send_reply
*/

#include "myftp.h"
#include <stdio.h>
#include <stdarg.h>

static const struct response RESPONSES[] = {
    {CMD_OK, "Command okay"CRLF},
    {ERR_CMD_UNKNOWN, "Syntax error, command unrecognized"CRLF},
    {SUCC_PASV_ENTER, "Entering Passive Mode.  (127,0,0,1,%d,%d)"CRLF},
    {ERR_CMD_NOT_IMPL, "Command not implemented"CRLF},
    {ERR_CMD_BAD_SEQ, "Bad sequence of commands"CRLF},
    {ERR_CMD_PARAM_NOT_IMPL, "Command not implemented for that parameter"CRLF},
    {RESTART_MARKER_REPLY, "MARK %s = %ld"CRLF},
    {STATUS_SYSTEM, "%s"CRLF},
    {STATUS_DIR, "%s"CRLF},
    {STATUS_FILE, "%s"CRLF},
    {STATUS_HELP, "%s"CRLF},
    {SERVICE_READY_LATER, "Service ready in %d minutes"CRLF},
    {SERVICE_READY, "Service ready for new user"CRLF},
    {SERVICE_LOGOUT, "Service closing TELNET connection"CRLF},
    {SERVICE_FORCED_LOGOUT, "%s"CRLF},
    {TRANSFER_DATA_ALREADY_OPEN, "Data connection already open; transfer "
        "starting"CRLF},
    {TRANSFER_DATA_OPEN, "Data connection open; no transfer in progress"CRLF},
    {ERR_CANT_OPEN_DATACON, "Can't open data connection"CRLF},
    {SUCC_DATACON_CLOSE, "Closing data connection; requested file "
        "action successful"CRLF},
    {ERR_CON_CLOSED, "Connection closed; transfer aborted."CRLF},
    {SUCC_LOGGED_IN, "User logged in, proceed"CRLF},
    {ERR_NOT_LOGGED_IN, "Not logged in"CRLF},
    {SUCC_NEED_PASSWORD, "User name okay, need password"CRLF},
    {NEED_ACCOUNT_FOR_LOGIN, "Need account for login"CRLF},
    {NEED_ACCOUNT_FOR_STORING_FILES, "Need account for storing files"CRLF},
    {FILE_STATUS_OKAY, "File status okay; "
        "about to open data connection."CRLF},
    {ERR_USER_NOT_LOCAL, "User not local; Will forward to %s@%s"CRLF},
    {ERR_USER_UNKNOWN, "User Unknown; Mail will be forwarded by the "
        "operator."CRLF},
    {SUCC_FILE_ACTION, "Requested file action okay, completed."CRLF},
    {PEND_FILE_ACTION, "Requested file action pending further"
        " information"CRLF},
    {ERR_ACTION_NOT_TAKEN_ACCESS, "Requested action not taken: "
        "file unavailable"CRLF},
    {ERR_ACTION_ABORTED_INTERNAL, "Requested action aborted: local error in "
        "processing"CRLF},
    {ERR_ACTION_ABORTED_PAGE_TYPE, "Requested action aborted: page type "
        "unknown"CRLF},
    {ERR_ACTION_NOT_TAKEN_STORAGE, "Requested action not taken: insufficient "
        "storage space in system"CRLF},
    {ERR_ACTION_ABORTED_STORAGE, "Requested file action aborted: exceed "
        "storage allocation"CRLF},
    {ERR_INVALID_FILE_NAME, "Requested action not taken: file name not "
        "allowed"CRLF},
    {MAIL_INPUT, "%s"},
    {SUCC_PATHNAME, "\"%s\""CRLF}
};

size_t send_ctrl_reply(client_t *client, int code, ...)
{
    static char buffer[1024];
    va_list ap;
    unsigned long int i;

    va_start(ap, code);
    for (i = 0; i < sizeof(RESPONSES) / sizeof(struct response); i++) {
        if (RESPONSES[i].code == code) {
            vsnprintf(buffer, sizeof(buffer), RESPONSES[i].msg, ap);
            printf("[Client %d] Server: %d %s", client->ctrl_socket,
                code, buffer);
            return printf_client(client, S_CTRL, "%d %s", code, buffer);
        }
    }
    va_end(ap);
    return -1;
}
