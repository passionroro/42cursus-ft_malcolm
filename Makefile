NAME 		=	ft_malcom
LIBFT 		= 	libft.a
LIB_DIR		=	libft/
OBJ_DIR		=	obj/
SRCS_DIR	=	src/
SRCS		=	main.c

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

LFLAGS = -I include/
CFLAGS = -Wall -Wextra -Werror
DEBUGGING = -g -O0
CC = gcc
RM = rm -rf
AR = ar rc

all: $(NAME)

$(OBJ_DIR)%.o : $(SRCS_DIR)%.c
		@mkdir -p $(OBJ_DIR)
		$(CC) -c $(CFLAGS) $(DEBUGGING) $(LFLAGS) $< -o $@

$(NAME): $(OBJS)
		@echo "Compilation of $(OBJ_DIR):  \033[1;32mOK\033[m\n"
		make -C $(LIB_DIR)
		@echo "Compilation of $(LIB_DIR):  \033[1;32mOK\033[m\n"
		$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(LIB_DIR)$(LIBFT) -o $(NAME)
		@echo "Compilation of $(NAME):  \033[1;32mOK\033[m\n"
		clear

clean:
		$(RM) $(OBJ_DIR)
		$(RM) $(OBJS)
		@echo "\033[1;31mObjects from $(NAME) removed\033[m"

fclean: clean
		$(RM) $(NAME)
		make -C $(LIB_DIR) fclean
		@echo "\033[1;31m$(NAME) and libft cleaned\033[m"

re: fclean all

.PHONY: all clean re fclean
