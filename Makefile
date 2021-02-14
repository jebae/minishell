NAME = minishell

# path
SRC_DIR = ./srcs
OBJ_DIR = ./objs
INC_DIR = ./includes
LIB_DIR = ./libs
LIBFT = $(LIB_DIR)/libft
FT_PRINTF = $(LIB_DIR)/ft_printf

# compiler option
CC = gcc
CFLAGS = -Wextra -Wall -Werror
INCLUDES = -I $(INC_DIR)\
	-I $(LIBFT)/includes\
	-I $(FT_PRINTF)/includes\

LIBS = -L $(LIBFT) -lft\
	-L $(FT_PRINTF) -lftprintf\

# srcs
SRC_MAIN = main.c

SRC_BUILTIN = echo.c\
	env.c\
	setenv.c\
	unsetenv.c\
	cd.c\
	dirs.c\
	exit.c\

SRC_PARSE = token.c\
	parse.c\
	parse_backslash.c\
	parse_dollar.c\
	parse_expr.c\
	parse_quote.c\
	parse_tilde.c\

SRC_SHELL = directory_stack.c\
	exec.c\
	get_executable.c\
	prompt.c\
	error.c\

SRC_UTILS = utils.c\

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_MAIN:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(SRC_BUILTIN:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(SRC_PARSE:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(SRC_SHELL:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(SRC_UTILS:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/builtin/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/parse/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/shell/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

TESTS = $(SRC_DIR)/**/*.test.cpp $(SRC_DIR)/*.test.cpp

# command
all : $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(FT_PRINTF)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(OBJS) -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

def = ''

test: $(filter-out srcs/main.c, $(wildcard srcs/*.c)) $(wildcard srcs/**/*.c) $(TESTS)
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(FT_PRINTF)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-lgtest\
		$(LIBS)\
		$(INCLUDES)\
		$(TESTS) $(filter-out srcs/main.c, $(wildcard srcs/*.c)) $(wildcard srcs/**/*.c)\
		-o $@

.PHONY: test all re clean fclean
