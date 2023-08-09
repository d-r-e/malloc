#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <libft.h>
#include <stdbool.h>

#define TINY 64
#define SMALL 1024
#define LARGE 1024 * 1024

/* TYPES AND STRUCTS */

typedef struct s_block
{
	size_t size;
	bool inuse;
	struct s_block *next;
	struct s_block *prev;
} t_block;

/* SUBJECT FUNCTIONS */

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

#endif