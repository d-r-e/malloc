#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <libft.h>
#include <stdbool.h>

#define TINY 32
#define SMALL 1024
#define LARGE 4096 * 4

#define N_BLOCKS getpagesize() / TINY // 128

/* TYPES AND STRUCTS */

typedef struct s_block
{
	size_t size;
	bool inuse;
	struct s_block *next;
	struct s_block *prev;
} t_block;

extern t_block *g_head;


/* AUX FUNCTIONS */
void show_alloc_mem_ex();


/* SUBJECT FUNCTIONS */

void *malloc(size_t size);
void free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

#endif