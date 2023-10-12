/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 00:42:42 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/12 12:04:42 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <libft.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>
# include <limits.h>

# define ALIGNMENT (16)

/* TYPES AND STRUCTS */
typedef struct s_block {
	size_t size;
	bool inuse;
	struct s_block *next;
	struct s_block *prev;
} t_block;

typedef struct s_heap {
	t_block *tiny;
	t_block *small;
	t_block *large;
} t_heap;


enum BlockType {
	BLOCK_TINY,
	BLOCK_SMALL,
	BLOCK_LARGE
};

# define ceil(x) ((x - (int)(x)) > 0 ? (int)(x + 1) : (int)(x))

#define PAGE_SIZE 4096

# ifndef N_BLOCKS
#  define N_BLOCKS 128
# endif

#define OVERHEAD (sizeof(t_block) + ALIGNMENT)

# define TINY (size_t)8
# define SMALL (size_t)(TINY * 16)

#define TINY_ARENA (ceil((double)(TINY * N_BLOCKS + OVERHEAD * N_BLOCKS) / PAGE_SIZE) * PAGE_SIZE)
#define SMALL_ARENA (ceil((double)(SMALL * N_BLOCKS + OVERHEAD * N_BLOCKS) / PAGE_SIZE) * PAGE_SIZE)



# define MMAP_ERROR_STRING "error: mmap failed\n"
# define MUNMAP_ERROR_STRING "error: received pointer was not properly allocated\n"

# define BLUE "\e[1;34m"
# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"
# define PURPLE "\e[1;35m"


extern t_heap g_heap;

/* AUX FUNCTIONS */
void show_alloc_mem_ex(void);
void show_alloc_mem(void);

/* SUBJECT FUNCTIONS */
void *malloc(size_t size);
void *realloc(void *mem, size_t size);
void free(void *mem);

#endif
