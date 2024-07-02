# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/26 17:15:13 by pschmunk          #+#    #+#              #
#    Updated: 2024/07/02 17:15:39 by pschmunk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -g -Wall -Wextra -Werror
RFLAG = -lreadline
RM = rm -rf

MINISHELL = minishell

MINISHELL_SRC =	./sources/minishell.c		\

MINISHELL_OBJ = $(MINISHELL_SRC:.c=.o)

all: $(MINISHELL)

$(MINISHELL): $(MINISHELL_OBJ)
	$(CC) $(CFLAGS) $(MINISHELL_OBJ) -o minishell $(RFLAG)

clean:
	$(RM) $(MINISHELL_OBJ)
fclean: clean
	$(RM) $(MINISHELL)
re: fclean all