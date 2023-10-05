/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:14:16 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/05 19:17:14 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

void defragment_block(t_block *block){
    t_block* ptr = block;

    if (block->next && block->next->next){
        (void)ptr;
    }
}

void free(void *ptr)
{
    t_block *block;
    int res;
    unsigned int clear_blocks;

    block = (t_block *)ptr - 1;
    block->inuse = false;
    
    if (block->size == LARGE)
    {
        if (block->prev)
            block->prev->next = block->next;
        else
            g_heap.large = block->next;
        
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
        res = munmap(g_heap.tiny, (sizeof(t_block) + TINY) * clear_blocks);
        if (res)
        {
            ft_putstr("munmap failed at free\n");
            exit(res);
    }
    g_heap.tiny = NULL;
    }
}
