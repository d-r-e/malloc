/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 00:42:42 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/08 14:05:11 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>
#include <libft.h>

static void *disalign_memory(void *mem, size_t alignment) {
	return (void *) ((size_t) mem & ~(alignment - 1));
}

void *realloc(void *mem, size_t size) {
	void *dst;
	void *actual_mem;
	t_block *tmp;

	if (!mem)
		return NULL;
	actual_mem = disalign_memory(mem, ALIGNMENT);
	tmp = (t_block *) actual_mem - 1;
	if (tmp->size != SMALL && tmp->size != TINY && tmp->size < LARGE)
		return NULL;
	if (size <= TINY && tmp->size == TINY)
		return mem;
	if (size > TINY && size <= SMALL && tmp->size == SMALL)
		return mem;
	dst = malloc(size);
	if (!dst)
		return NULL;
	if (size < tmp->size)
		ft_memcpy(dst, mem, size);
	else
		ft_memcpy(dst, mem, tmp->size);
	free(mem);
	return dst;
}