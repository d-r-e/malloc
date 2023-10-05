CFLAGS = -Wall -Wextra -Werror -g3
CC = gcc
CPPFLAGS = -I $(INC) -I libft
SRC = src/malloc.c src/output.c src/free.c
INCLUDE = inc
HEADER = inc/ft_malloc.h
OBJ = $(SRC:.c=.o)
TEST_SRC = test_src/main.c
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
	$(CC) $(CFLAGS) -c $< -I libft -I $(INCLUDE) -fPIC -o $@

$(LIBFT):
	$(MAKE) -C libft
	echo $@ created

clean:
	$(MAKE) -C libft clean
	$(RM) $(OBJ)



test: $(LIBRARY) $(TEST_SRC) $(HEADER)
	$(CC) $(CFLAGS) -o ./test $(TEST_SRC) -I $(INCLUDE)  -I libft  -L libft -lft -L . -lmalloc


x: test
	@ulimit -n 4096
	@LD_LIBRARY_PATH=. ./test
	@$(RM) ./test

p: poetry

poetry: $(NAME) test_src/poetry.c
	$(CC) $(CFLAGS) -o ./test test_src/poetry.c -I $(INCLUDE)  -I libft  -L libft -lft -L . -lmalloc
	./test
unit: $(NAME)
	$(CC) $(CFLAGS) -o test test_src/unit_test.c -I libft -L libft -lft -I $(INCLUDE) -L . -lmalloc
	@ulimit -n 4096
	LD_LIBRARY_PATH=. ./test
	rm -f test

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(LIBRARY) $(NAME)

re: fclean all

.PHONY: all clean fclean re
