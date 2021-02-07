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

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_MAIN:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(SRC_BUILTIN:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/builtin/%.c

TESTS = $(SRC_DIR)/**/*.test.cpp $(SRC_DIR)/*.test.cpp

def = ''

test: $(addprefix $(SRC_DIR)/builtin/, $(SRC_BUILTIN)) $(TESTS)
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(FT_PRINTF)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-lgtest\
		$(LIBS)\
		$(INCLUDES)\
		$(TESTS) $(addprefix $(SRC_DIR)/builtin/, $(SRC_BUILTIN))\
		-o $@

.PHONY: test
