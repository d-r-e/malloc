CFLAGS = -Wall -Wextra -Werror -Wformat-security -pedantic
CC = gcc
SRC = src/malloc.c
INC = inc
OBJ = $(SRC:.c=.o)

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libmalloc.so
LIBRARY = libmalloc_$(HOSTTYPE).so
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(LIBRARY)
	@ln -fs $(LIBRARY) $(NAME)
	@echo -e "\033[32m[ ✔ ] $@ created\033[0m"

$(LIBRARY): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -shared -o $@ $(OBJ) $(LIBFT)

$(OBJ): $(SRC) $(LIBFT)
	@$(CC) $(CFLAGS) -c $(SRC) -l $(LIBFT) -L libft -I $(INC) -fPIC -o $(OBJ)

$(LIBFT):
	@make -C libft
	@echo -e "\033[32m[ ✔ ] $@ created\033[0m"

clean:
	make -C libft clean
	rm -f $(OBJ)

test: $(LIBRARY)
	$(CC) $(CFLAGS) -o test.out test/main.c -lft -L libft -I $(INC) -L . -lmalloc
	LD_LIBRARY_PATH=. valgrind --leak-check=full ./test.out
	rm -f test.out

fclean: clean
	make -C libft fclean
	rm -f $(LIBRARY) $(NAME)

re: fclean all

.PHONY: all clean fclean re test
