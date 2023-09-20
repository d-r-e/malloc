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
SYMLINK = ln -fs
RM = rm -f


all: $(NAME)

$(NAME): $(LIBRARY) $(SRC)
	$(SYMLINK) $(LIBRARY) $(NAME)
	@echo $@ created

$(LIBRARY): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ) $(LIBFT)

%.o: %.c $(HEADER) $(LIBFT)
	$(CC) $(CFLAGS) -c $< -I libft -I $(INC) -fPIC -o $@

$(LIBFT):
	make -C libft
	echo $@ created

clean:
	$(MAKE) -C libft clean
	$(RM) $(OBJ)



test: $(LIBRARY) test_src/main.c
	$(CC) $(CFLAGS) -o ./test test_src/main.c -I $(INC)  -I libft  -L libft -lft -L . -lmalloc


x: test
	@ulimit -n 4096
	@LD_LIBRARY_PATH=. ./test
	@$(RM) ./test


unit: $(NAME)
	$(CC) $(CFLAGS) -o test test_src/unit_test.c -I libft -L libft -lft -I $(INC) -L . -lmalloc
	@ulimit -n 4096
	LD_LIBRARY_PATH=. ./test
	rm -f test

fclean: clean
	make -C libft fclean
	rm -f $(LIBRARY) $(NAME)

re: fclean all

.PHONY: all clean fclean re
