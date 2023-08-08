#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif