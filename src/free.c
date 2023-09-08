#include <ft_malloc.h>

void free(void *ptr)
{
    t_block *block;
    int res;


    block = (t_block *)ptr - 1;
    block->inuse = false;
    if (block->size == LARGE){
       res = munmap((void*)block, sizeof(t_block) + block->size);
       if (res){
        ft_puts(FREE_ERROR_STRING);
        exit(-1);
       }
    }

    t_block *tmp = g_head;
    while (tmp->next){
        if (tmp->inuse)
            return;
        tmp = tmp->next;
    }
    res = munmap(g_head, calculate_total_memory());
    if (res){
        printf("munmap failed at free\n");
        exit(res);
    }
}
