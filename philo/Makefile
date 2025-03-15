# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/08 15:40:01 by mait-you          #+#    #+#              #
#    Updated: 2025/03/15 12:22:11 by mait-you         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### Compiler and Flags
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -fsanitize=thread -g 
RM			:= rm -rf

### dirs

OBJS_DIR	:= obj

### Executable Name
NAME		:= philo

### Source Files
SRCS		:= main.c cleanup.c utils.c init.c parsing.c philosopher.c monitor.c

### Object Files
OBJS		:= $(SRCS:%.c=$(OBJS_DIR)/%.o)

### Colors and Formatting
RESET		:= \033[0m
RED			:= \033[31m
GREEN		:= \033[32m
BLUE		:= \033[34m
WHITE		:= \033[37m
BOLD		:= \033[1m
UP			:= \033[F

### Rules
all: $(NAME)

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(RED)$(BOLD)$(NAME) $(GREEN)compiled successfully!$(RESET)"

# mandatory Object file compilation rules
$(OBJS_DIR)/%.o: %.c philo.h | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling: $(WHITE)$<$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(RED)Cleaned object files.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)$(UP)Cleaned object files and $(PURPLE)$(BOLD)$(LIBFT)$(RESET)$(RED), $(RED)$(BOLD)$(NAME)$(RESET)"

re: fclean all

.PHONY: clean
