#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <libft.h>

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

#endif