/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 17:13:48 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/08 11:39:15 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

static void *disalign_memory(void *mem, size_t alignment)
{
    return (void *)((size_t)mem & ~(alignment - 1));
}

void * realloc(void *mem, size_t size){
    void *dst;
    t_block *tmp;

    // disalign memory
    mem = disalign_memory(mem, ALIGNMENT);
    tmp = (t_block *)mem - 1;
    if (tmp->size != SMALL && tmp->size != TINY && tmp->size < LARGE)
        return NULL;
    dst = malloc(size);
    if (!dst)
        return NULL;
    ft_memcpy(dst, mem, size);
    free(mem);
    return dst;

}