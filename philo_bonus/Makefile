CC			= cc
CFLAGS		= -Wall -Wextra -Werror 
RM			= rm -rf
NAME		= philo_bonus
OBJS_DIR 	= obj

SRCS =	main_bonus.c \
	src_bonus/monitor_bonus.c \
	src_bonus/init_bonus.c \
	src_bonus/parsing_bonus.c \
	src_bonus/cleanup_bonus.c \
	src_bonus/philosophers_bonus.c \
	src_bonus/utils_bonus.c \
	src_bonus/utils_2_bonus.c \
	src_bonus/utils_3_bonus.c

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

$(OBJS_DIR)/%.o: %.c include_bonus/philo_bonus.h
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