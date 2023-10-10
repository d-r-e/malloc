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

void clear_chunk(size_t size){
	t_block *ptr;
	size_t counter = 0;

	if (size <= TINY)
		ptr = g_heap.tiny;
	else if (size <= SMALL)
		ptr = g_heap.small;
	else
		ptr = g_heap.large;
	while (ptr) {
		if (ptr->inuse == false)
			counter++;
		if (counter == N_BLOCKS) {
			if (size <= TINY){
				if (ptr && ptr->next)
					ptr = ptr->next;
				munmap((void *) g_heap.tiny, N_BLOCKS * (sizeof(t_block ) + TINY + ALIGNMENT));
				g_heap.tiny = ptr;
			}
			else if (size <= SMALL){
				if (ptr->next)
					ptr = ptr->next;
				munmap((void *) g_heap.small, N_BLOCKS * (sizeof(t_block ) + SMALL + ALIGNMENT));
				g_heap.small = ptr;
			}


			break;
		}
		ptr = ptr->next;
	}

}

void free(void *ptr) {
	t_block *block;
	int ret = 0;

	if (!ptr)
		return;
	ptr = disalign_memory(ptr, ALIGNMENT);
	block = (t_block *) ptr - 1;
	block->inuse = false;

	if (block->size == LARGE) {
		if (block->prev)
			block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		if (!block->prev)
			g_heap.large = block->next;

		if (munmap((void *) block, sizeof(t_block) + block->size + ALIGNMENT)) {
			ft_puts(MUNMAP_ERROR_STRING);
			ret = -1;
			exit(ret);
		}
	}

//	clear_chunk(block->size);

//	else if (block->size == TINY && check_last_n_blocks_not_inuse(g_heap.tiny)) {
//
//		ret = munmap((void *) g_heap.tiny, N_BLOCKS * (sizeof(t_block *) + TINY));
//		if (ret) {
//			ft_puts(MUNMAP_ERROR_STRING);
//			exit(-1);
//		}
//		g_heap.tiny = NULL;
//	} else if (block->size == SMALL && check_last_n_blocks_not_inuse(g_heap.small)) {
//		ret = munmap((void *) g_heap.small, N_BLOCKS * (sizeof(t_block *) + SMALL));
//		if (ret) {
//			ft_puts(MUNMAP_ERROR_STRING);
//			exit(-1);
//		}
//		g_heap.small = NULL;
//	}
}
