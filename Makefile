# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/26 17:44:44 by brheaume          #+#    #+#              #
#    Updated: 2023/06/26 17:57:41 by brheaume         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = time.c \
	  utils.c \
	  main.c \
	  angel.c \
	  alive.c \
	  verify.c \
	  seeker.c \
	  annihilation.c \
	  initialization.c \

OBJ = ${SRC:.c=.o}

CC = clang
CFLAGS = -Wextra -Wall -Werror

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

sani: CFLAGS += -fsanitize=thread
sani: clean all

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
