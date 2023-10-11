/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 00:42:42 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/10 18:16:52 by darodrig         ###   ########.fr       */
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

# define ALIGNMENT (8)

# define TINY (size_t)(4096 / (sizeof(t_block) + 128  + ALIGNMENT ))
# define SMALL (size_t)(4096 / (ALIGNMENT + sizeof(t_block) + 64 ))
# define LARGE (size_t)(4096 / (ALIGNMENT + sizeof(t_block) + 16 ))
//# define TINY 4
//# define SMALL (TINY * 4)
//# define LARGE (size_t)(TINY * 16)

# ifndef N_BLOCKS
#  define N_BLOCKS 128
# endif

#define TINY_ARENA (((N_BLOCKS * TINY) + sizeof(t_block)) * N_BLOCKS )
#define SMALL_ARENA (((N_BLOCKS * SMALL) + sizeof(t_block)) * N_BLOCKS )


# define MUNMAP_ERROR_STRING "malloc: error: munmap: Could not free addressed memory."



# define BLUE "\e[1;34m"
# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"
# define PURPLE "\e[1;35m"

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

extern t_heap g_heap;

/* AUX FUNCTIONS */
void show_alloc_mem_ex(void);
void show_alloc_mem(void);

/* SUBJECT FUNCTIONS */
void *malloc(size_t size);
void *realloc(void *mem, size_t size);
void free(void *mem);

#endif
