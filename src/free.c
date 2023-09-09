#include <ft_malloc.h>

void free(void *ptr)
{
    t_block *block;
    int res;
    unsigned int clear_blocks;

    block = (t_block *)ptr - 1;
    block->inuse = false;
    if (block->size == LARGE)
    {
        res = munmap((void *)block, sizeof(t_block) + block->size);
        if (res)
        {
            ft_puts(FREE_ERROR_STRING);
            exit(-1);
        }
    }

    t_block *tmp = g_heap.tiny;
    clear_blocks = 0;
    while (tmp)
    {
        if (tmp->inuse)
            return;
        clear_blocks++;
        tmp = tmp->next;
    }
    if (clear_blocks == N_BLOCKS)
    {
        res = munmap(g_heap.tiny, (sizeof(t_block) + TINY) * N_BLOCKS);
        if (res)
        {
            ft_putstr("munmap failed at free\n");
            exit(res);
    }
    g_heap.tiny = NULL;
    }
}
