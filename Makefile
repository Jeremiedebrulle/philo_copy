# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/27 13:10:01 by jdebrull          #+#    #+#              #
#    Updated: 2025/07/01 13:16:31 by jdebrull         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCDIR = srcs/
OBJDIR = objs
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -fsanitize=thread

SRC = 	main.c \
		utils.c

SRCS = $(addprefix $(SRCDIR), $(SRC))
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
