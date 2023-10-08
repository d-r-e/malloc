#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <libft.h>
#include <stdint.h>

#define TINY 32
#define SMALL (TINY * 4) // 128
#define LARGE (SMALL * 16) // 2048

#define ALIGNMENT 8
// #define TINY 8
// #define SMALL 16
// #define LARGE 32

# define N_BLOCKS 4

# define MUNMAP_ERROR_STRING "malloc: error: munmap: Could not free addressed memory."

# ifndef    M_MMAP_THRESHOLD
#  define   M_MMAP_THRESHOLD 131072
# endif

# define BLUE "\e[1;34m"
# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"
# define PURPLE "\e[1;35m"

/* TYPES AND STRUCTS */
typedef struct s_block
{
	size_t			size;
	bool			inuse;
	struct s_block	*next;
	struct s_block	*prev;
}	t_block;

typedef struct s_heap {
	t_block	*tiny;
	t_block	*small;
	t_block	*large;
}	t_heap;

extern t_heap	g_heap;

/* AUX FUNCTIONS */
void	show_alloc_mem_ex(void);
void	show_alloc_mem(void);

/* SUBJECT FUNCTIONS */
void	*malloc(size_t size);
void	*realloc(void *mem, size_t size);
void	free(void *mem);

#endif
