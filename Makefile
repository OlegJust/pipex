# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opidhorn <opidhorn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/01 14:43:15 by opidhorn          #+#    #+#              #
#    Updated: 2025/02/01 14:43:16 by opidhorn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Имя выходного файла
NAME = pipex

# Компилятор и флаги
CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibft

# Файлы исходников и объектов
SRCS = pipex.c utils.c
OBJS = $(SRCS:.c=.o)

# Путь к libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Основные цели
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

# Компиляция исходников
%.o: %.c pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка файлов объектных файлов
clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

# Полная очистка проекта
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Полная пересборка проекта
re: fclean all

.PHONY: all clean fclean re
