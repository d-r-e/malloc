/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:14:16 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/08 11:30:01 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

static void *disalign_memory(void *mem, size_t alignment)
{
    return (void *)((size_t)mem & ~(alignment - 1));
}

// static
// void defragment_block(t_block *block){
//     t_block* ptr = block;

//     if (block->next && block->next->next){
//         (void)ptr;
//     }
// }

void empty_heap(t_block *mem){
    unsigned int free_blocks = 0;
    t_block *tmp;
    int res;

    tmp = mem;
    for (unsigned int i = 0; i < N_BLOCKS; i++)
    {
        if (!tmp->inuse)
            free_blocks++;
        tmp = tmp->next;
    }
    if (free_blocks == N_BLOCKS)
    {
        res = munmap((void *)mem, (sizeof(t_block) + mem->size) * N_BLOCKS);
        if (res)
        {
            ft_puts(MUNMAP_ERROR_STRING);
            exit(-1);
        }
        mem = NULL;
    }
}

// t_block *find_nearest_heap_start(t_block *block)
// {
//     // Going at most N_BLOCKS blocks ahead, find  the nearest heap start from the back of the list
//     // returns the N_BLOCKS % 0 block
//     t_block *tmp = block;
//     unsigned int blocks_ahead = 0;
//     unsigned int blocks_behind = 0;

//     while (tmp->next && blocks_ahead < N_BLOCKS)
//     {
//         tmp = tmp->next;
//         blocks_ahead++;
//     }
    
// }

void free(void *ptr)
{
    t_block *block;
    int err;

    if (!ptr)
        return;
    ptr = disalign_memory(ptr, ALIGNMENT);
    block = (t_block *)ptr - 1;
    block->inuse = false;
    
    if (block->size == LARGE)
    {
        if (block->prev)
            block->prev->next = block->next;
        else
            g_heap.large = block->next;
        block->next->prev = block->prev;
        err = munmap((void *)block, sizeof(t_block) + block->size + ALIGNMENT);
        if (err)
        {
            ft_puts(MUNMAP_ERROR_STRING);
            exit(-1);
        }
          block->inuse = false;
          err = 0;

    } else {
        block->inuse = false;
        
    }

    return; 
    
}
