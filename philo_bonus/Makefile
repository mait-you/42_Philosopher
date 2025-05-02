### Compiler and Flags
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
RM			:= rm -rf

### dirs
OBJS_DIR	:= obj

### Executable Name
NAME		:= philo_bonus

### Source Files
SRCS =	main_bonus.c \
	src/monitor_bonus.c \
	src/init_bonus.c \
	src/parsing_bonus.c \
	src/cleanup_bonus.c \
	src/philosophers_bonus.c \
	src/utils_bonus.c \

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

$(OBJS_DIR)/%.o: %.c include/philo_bonus.h | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling: $(WHITE)$<$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(RED)Cleaned object files.$(RESET)"

fclean:
	@$(RM) $(NAME) $(OBJS_DIR)
	@echo "$(RED)$(UP)Cleaned object files and $(PURPLE)$(BOLD)$(LIBFT)$(RESET)$(RED), $(RED)$(BOLD)$(NAME)$(RESET)"

re: fclean all

.PHONY: clean fclean
