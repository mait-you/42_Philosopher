CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf
OBJS_DIR	= obj
NAME		= philo

SRCS =	main.c \
	src/monitor.c \
	src/init.c \
	src/parsing.c \
	src/cleanup.c \
	src/philosophers.c \
	src/utils.c 
OBJS		= $(SRCS:%.c=$(OBJS_DIR)/%.o)

RESET		= \033[0m
RED			= \033[31m
GREEN		= \033[32m
BLUE		= \033[34m
WHITE		= \033[37m
BOLD		= \033[1m

all: $(NAME)

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(RED)$(BOLD)$(NAME) $(GREEN)compiled successfully!$(RESET)"

$(OBJS_DIR)/%.o: %.c include/philo.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling: $(WHITE)$<$(RESET)"

clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(RED)Cleaned object files.$(RESET)"

fclean:
	@$(RM) $(NAME) $(OBJS_DIR)
	@echo "$(RED)Cleaned object files and $(PURPLE)$(BOLD)$(LIBFT)$(RESET)$(RED), $(RED)$(BOLD)$(NAME)$(RESET)"

re: fclean all