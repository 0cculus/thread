SRC = main.c

CC = clang
CFLAGS = -Wextra -Wall -Werror

all:
	$(CC) $(CFLAGS) $(SRC)

sani: CFLAGS += -fsanitize=thread -g
sani: all
