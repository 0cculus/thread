SRC = main.c

all:
	clang -Wextra -Wall -Werror $(SRC)
