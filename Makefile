# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbahry <cbahry@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/15 22:33:30 by cbahry            #+#    #+#              #
#    Updated: 2026/04/15 22:33:32 by cbahry           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -std=c89
SRC         = main.c \
              init.c \
              parsing.c \
              utils.c \
              utils_2.c \
              time.c \
              logging.c \
              routines.c \
              routines_utils.c \
              dongle_operations.c \
              dongle_operations_utils.c \
              min_heap.c \
              monitor.c \
              cleanup.c \

OBJ         = $(SRC:.c=.o)
HEADER      = codexion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
