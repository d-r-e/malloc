CFLAGS = -Wall -Wextra -Werror -Wformat-security -pedantic -g
CC = gcc
SRC = src/malloc.c src/output.c src/free.c
INC = inc
HEADER = inc/ft_malloc.h
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
	@echo "\033[32m[ ✔ ] $@ created\033[0m"

$(LIBRARY): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -shared -o $@ $(OBJ) $(LIBFT)

%.o: %.c $(HEADER) $(LIBFT)
	@$(CC) $(CFLAGS) -c $< -I libft -I $(INC) -fPIC -o $@

$(LIBFT):
	@make -C libft
	@echo "\033[32m[ ✔ ] $@ created\033[0m"

clean:
	make -C libft clean
	rm -f $(OBJ)

test: $(LIBRARY)
	$(CC) $(CFLAGS) -o test.out test/main.c -I $(INC)  -I libft  -L libft -lft -L . -lmalloc
	@ulimit -n 4096
	LD_LIBRARY_PATH=. valgrind -s --leak-check=full ./test.out
	rm -f test.out

x: $(NAME)
	$(CC) $(CFLAGS) -o test.out test/main.c -I libft -L libft -lft  -I $(INC) -L . -lmalloc
	@ulimit -n 4096
	@LD_LIBRARY_PATH=. ./test.out
	@rm -f test.out

fclean: clean
	make -C libft fclean
	rm -f $(LIBRARY) $(NAME)

re: fclean all

.PHONY: all clean fclean re test
