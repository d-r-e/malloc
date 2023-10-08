/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:14:16 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/08 14:04:31 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

static void *disalign_memory(void *mem, size_t alignment) {
	return (void *) ((size_t) mem & ~(alignment - 1));
}

void free(void *ptr) {
	t_block *block;

	if (!ptr)
		return;
	ptr = disalign_memory(ptr, ALIGNMENT);
	block = (t_block *) ptr - 1;
	block->inuse = false;
	if (block->size == LARGE){
		if (block->prev)
			block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		if (!block->prev)
			g_heap.large = block->next;
		if (munmap((void *) block, sizeof(t_block) + block->size + ALIGNMENT)) {
			ft_puts(MUNMAP_ERROR_STRING);
			exit(-1);
		}
	}
}
