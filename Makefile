CFLAGS = -Wall -Wextra -Werror
CC = gcc
SRC = src/malloc.c
INC = inc
OBJ = $(SRC:.c=.o)
NAME = libmalloc-linux-x86_64.so
LIBFT = libft/libft.a

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ) $(LIBFT)
	@echo -e "\033[32m[ ✔ ] $(NAME) created\033[0m"

$(OBJ): $(LIBFT) $(SRC)
	@$(CC) $(CFLAGS) -c $(SRC) -l $(LIBFT) -L libft -I $(INC) -fPIC -o $(OBJ)


all: $(NAME)

clean:
	@rm -f $(OBJ)
	@echo -e "\033[31m[ ✔ ] $(NAME) objects deleted\033[0m"

$(LIBFT):
	@make -C libft

fclean: clean
	@rm -f $(NAME)
	@echo -e "\033[31m[ ✔ ] $(NAME) deleted\033[0m"

re: fclean all

.PHONY: all clean fclean re