##
## EPITECH PROJECT, 2020
## myftp
## File description:
## Makefile
##

CC = gcc

AR = ar rc

CFLAGS = -fPIC -W -Wall -Werror -I./include -g3

CFLAGS_TEST = $(CFLAGS) -Wno-stringop-truncation -Wno-error=format --coverage

LFLAGS = \

LFLAGS_TEST = $(LFLAGS) -lcriterion

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

SRC =	src/util_ip.c \
		src/util_ip2.c \
		src/util_crlf.c \
		src/util_error.c \
		src/util_path.c \
		src/util_path_multiarg.c \
		src/util_path_relative.c \
		src/server.c \
		src/cmd.c \
		src/cmd_pasv.c \
		src/cmd_user.c \
		src/cmd_pass.c \
		src/cmd_quit.c \
		src/cmd_pwd.c \
		src/cmd_noop.c \
		src/cmd_help.c \
		src/cmd_port.c \
		src/cmd_cwd.c \
		src/cmd_cdup.c \
		src/cmd_dele.c \
		src/cmd_list.c \
		src/cmd_retr.c \
		src/cmd_stor.c \
		src/client_send_reply.c \
		src/client_write.c \
		src/client_read.c \
		src/client_disconnect.c \
		src/client_socket.c \
		src/client_ctrl.c \
		src/client_passive_data.c \
		src/client_active_data.c \
		src/client_data.c \

TEST_FILES =	\

OBJ = $(SRC:.c=.o)

TARGET = myftp

TARGET_TEST = unit_tests

all: build_all

build_all: $(OBJ)
	gcc -o $(TARGET) $(OBJ)

tests_run: clean_tests
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -o $(TARGET_TEST) $(SRC) $(TEST_FILES) \
		$(LFLAGS)
	./$(TARGET_TEST) --verbose

clean_tests:
	rm -rf $(TARGET_TEST)

coverage: tests_run
	gcovr

clean:
	rm -f $(OBJ) *.gc* unit_test

fclean: clean
	rm -f $(TARGET)

re: fclean all
