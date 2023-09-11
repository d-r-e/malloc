#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <libft.h>

#define TINY 16
#define SMALL 128
#define LARGE 4096 * 4

#define N_BLOCKS 128

#define FREE_ERROR_STRING "error: munmap: Could not free addressed memory."

#ifndef M_MMAP_THRESHOLD
#define M_MMAP_THRESHOLD 128 * 1024
#endif

/* TYPES AND STRUCTS */
typedef struct s_block
{
	size_t size;
	bool inuse;
	struct s_block *next;
	struct s_block *prev;
} t_block;

typedef struct s_heap {
	t_block * tiny;
	t_block *small;
	t_block *large;
} t_heap;

extern t_heap g_heap;

/* AUX FUNCTIONS */
void show_alloc_mem_ex();
void print_tblock_header();
void print_hex_tblock_body(t_block *block);
size_t calculate_total_memory();


/* SUBJECT FUNCTIONS */
void *malloc(size_t size);
void free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

#endif
