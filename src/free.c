/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:14:16 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/12 12:03:53 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

static
void *disalign_memory(void *mem, size_t alignment) {
	return (void *) ((size_t) mem & ~(alignment - 1));
}

static
void clear_chunk(size_t size) {
	t_block *ptr;
	size_t counter = 0;
	size_t i = 0;
	int ret = 0;

	if (size <= TINY)
		ptr = g_heap.tiny;
	else if (size <= SMALL)
		ptr = g_heap.small;
	else
		ptr = g_heap.large;
	while (ptr && i < N_BLOCKS) {
		if (ptr->inuse == false)
			counter++;
		else
			break;
		if (counter == N_BLOCKS) {
			if (size <= TINY) {
				ptr = ptr->next;
				ret = munmap((void *) g_heap.tiny, TINY_ARENA);
				if (ret) {
					dprintf(2, "Error: %s for size %zu\n", MUNMAP_ERROR_STRING, size);
					exit(253);
				}
				g_heap.tiny = ptr;
			} else if (size <= SMALL) {
				if (ptr->next)
					ptr = ptr->next;
				else ptr=NULL;
				ret = munmap((void *) g_heap.small, SMALL_ARENA);
				if (ret) {
					dprintf(2, "Error: %s for size %zu\n", MUNMAP_ERROR_STRING, size);
					exit(254);
				}

				g_heap.small = ptr;
			}

			break;
		}
		++i;
		ptr = ptr->next;
	}
}

static
bool is_block_allocated(t_block *block){
	t_block *tmp;

	tmp = g_heap.tiny;
	while (tmp && block->size == TINY) {
		if (tmp == block)
			return true;
		tmp = tmp->next;
	}
	tmp = g_heap.small;
	while (tmp && block->size ==  SMALL) {
		if (tmp == block)
			return true;
		tmp = tmp->next;
	}
	tmp = g_heap.large;
	while (tmp && block->size > SMALL) {
		if (tmp == block)
			return true;
		tmp = tmp->next;
	}
	return false;
}


/// \brief Frees a memory block
/// \author Darodrig
/// \details
///   The free() function deallocates the memory allocation pointed to by ptr.
///   If ptr is a NULL pointer, no operation is performed.
/// \param ptr
void free(void *ptr) {
	t_block *block;

	if (!ptr)
		return;
	ptr = disalign_memory(ptr, ALIGNMENT);
	block = (t_block *) ptr - 1;
	if (block->inuse == false || !is_block_allocated(block))
		return;


	if (block) {
		if (block->size > SMALL && block->size <= SIZE_MAX) {
			if (block->prev)
				block->prev->next = block->next ? block->next : NULL;
			else
				g_heap.large = block->next ? block->next : NULL;

			if (block->next)
				block->next->prev = block->prev;
			block->inuse = false;
			if (munmap((void *) block, block->size + OVERHEAD)) {
				dprintf(2, "free: %s for size %lu\n", MUNMAP_ERROR_STRING, block->size);
				exit(252);
			}
		} else if (block) {
			clear_chunk(block->size);
		}
	}
}
