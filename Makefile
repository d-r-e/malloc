CFLAGS = -Wall -Wextra -Werror -O2 -g
CC = gcc
CPPFLAGS = -I $(INC) -I libft
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

$(NAME): $(LIBRARY) $(SRC)
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



test: $(LIBRARY) test_src/main.c
	$(CC) $(CFLAGS) -o ./test test_src/main.c -I $(INC)  -I libft  -L libft -lft -L . -lmalloc


x: $(NAME) test
	@ulimit -n 4096
	@LD_LIBRARY_PATH=. ./test


unit: $(NAME)
	$(CC) $(CFLAGS) -o test test_src/unit_test.c -I libft -L libft -lft -I $(INC) -L . -lmalloc
	@ulimit -n 4096
	@LD_LIBRARY_PATH=. ./test
	@rm -f test

fclean: clean
	make -C libft fclean
	rm -f $(LIBRARY) $(NAME)

re: fclean all

.PHONY: all clean fclean re
