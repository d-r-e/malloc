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

//static int check_last_n_blocks_not_inuse(t_block *block) {
//	int count = 0;
//	while (block->next) {
//		block = block->next;
//	}
//	while (block && count < N_BLOCKS) {
//		if (block->inuse)
//			return 0;
//		block = block->prev;
//		count++;
//	}
//#if MALLOC_DEBUG
//	printf("check_last_n_blocks_not_inuse: %d\n", count);
//#endif
//	return (count == N_BLOCKS);
//}

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
