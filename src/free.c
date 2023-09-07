#include <ft_malloc.h>

void free(void *ptr)
{
    t_block *block;

    block = (t_block *)ptr - 1;
    block->inuse = false;
}
