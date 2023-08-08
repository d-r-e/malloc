#include <ft_malloc.h>
#include <stdlib.h>

void *ft_malloc(size_t size)
{
    void *ptr = NULL;
    int page_size = getpagesize();
    // struct rlimit limit;

    (void)ptr;
// printf("page size: %d\n", page_size);
// getrlimit(RLIMIT_AS, &limit);
// printf("limit: %lu\n", limit.rlim_cur);
#ifdef DEBUG
    printf("Malloc'ing %lu bytes 😜 \n", size);
#endif
    if (size == 0)
    {
        size = page_size;
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
#ifdef DEBUG

        printf("Correctly allocated %lu bytes 😜 \n", size);
#endif
        return ptr;
    }
    return malloc(size);
}

void ft_free(void *ptr)
{
    free(ptr);
}

void *ft_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}