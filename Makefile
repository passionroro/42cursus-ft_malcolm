NAME 		=	ft_malcolm
LIBFT 		= 	libft.a
LIB_DIR		=	libft/
OBJ_DIR		=	obj/
SRCS_DIR	=	src/
SRCS		=	main.c				\
				error.c				\
				parsing.c 			\
				init_sockets.c		\
				process_packets.c	\
				spoof_arp_reply.c	\
				utils/utils.c		\
				utils/print.c 

TEST_DIR	= src/tests/
TEST_SRCS	= test_main.c test_parsing.c
TEST_NAME	= run_tests

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
TEST_OBJS = $(addprefix $(OBJ_DIR), $(TEST_SRCS:.c=.o))

LFLAGS = -I include/
CFLAGS = -Wall -Wextra -Werror
DEBUGGING = -g -O0

CC = gcc
RM = rm -rf
AR = ar rc

all: $(NAME)

$(OBJ_DIR)%.o : $(SRCS_DIR)%.c
		@mkdir -p $(OBJ_DIR)
		@mkdir -p $(OBJ_DIR)/utils
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
		$(RM) $(TEST_NAME)
		make -C $(LIB_DIR) fclean
		@echo "\033[1;31m$(NAME) and libft cleaned\033[m"

re: fclean all

# Unit tests
$(OBJ_DIR)%.o : $(TEST_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $(DEBUGGING) $(LFLAGS) $< -o $@

$(TEST_NAME): $(filter-out $(OBJ_DIR)main.o, $(OBJS)) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ $(LFLAGS) $(LIB_DIR)$(LIBFT) -o $@

test: $(TEST_NAME)
	./$(TEST_NAME)

.PHONY: all clean re fclean
