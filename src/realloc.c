/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 17:13:48 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/05 19:01:21 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

void * realloc(void *mem, size_t size){
    void *dst;
    t_block *block_ptr;
    
    if (!mem || size <= 0)
        return malloc(size);
    dst = malloc(size);
    if (!dst)
        return mem;
    ft_memcpy(mem, dst, size);
    free(mem);
    return dst;
}