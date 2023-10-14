/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 00:42:42 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/12 11:48:02 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>
#include <libft.h>

static void *disalign_memory(void *mem, size_t alignment) {
	return (void *) ((size_t) mem & ~(alignment - 1));
}

static
bool is_allocated(t_block *ptr) {
	t_block *tmp = NULL;

	tmp = g_heap.tiny;
	while (tmp && ptr->size == TINY) {
		if (tmp == ptr)
			return true;
		tmp = tmp->next;
	}
	tmp = g_heap.small;
	while (tmp && ptr->size ==  SMALL) {
		if (tmp == ptr)
			return true;
		tmp = tmp->next;
	}
	tmp = g_heap.large;
	while (tmp && ptr->size > SMALL) {
		if (tmp == ptr)
			return true;
		tmp = tmp->next;
	}
	return false;
}

static bool is_inside_heap(void *ptr){

	void *tmp = g_heap.tiny;
	t_block *block = NULL;

	while (tmp) {
		block = tmp;
		if (ptr >= (void *) block && ptr <= (void *) block + TINY_ARENA)
			return true;
		tmp = block->next;
	}

	tmp = g_heap.small;

	while (tmp) {
		block = tmp;
		if (ptr >= (void *) block && ptr <= (void *) block + SMALL_ARENA)
			return true;
		tmp = block->next;
	}

	tmp = g_heap.large;

	while (tmp) {
		block = tmp;
		if (ptr >= (void *) block && ptr <= (void *) block + block->size + OVERHEAD)
			return true;
		tmp = block->next;
	}
	return false;
}

/// \brief
///  Reallocates a memory block
/// \author Darodrig
/// \details
///  The realloc() function tries to change the size of the allocation pointed
/// to by ptr to size, and returns ptr. If there is not enough room to enlarge
/// the memory allocation pointed to by ptr, realloc() creates a new allocation,
/// copies as much of the old data pointed to by ptr as will fit to the new
/// allocation, frees the old allocation, and returns a pointer to the allocated
/// memory.  If ptr is NULL, realloc() is identical to a call to malloc() for size bytes.
/// If size is zero and ptr is not NULL, a new, minimum sized object is allocated and
/// the original object is freed.
/// \param mem pointer to the memory
/// \param size in bytes
void *realloc(void *mem, size_t size) {
	t_block *ptr = NULL;

	if (!mem)
		return malloc(size);
	if ((size_t) mem % ALIGNMENT != 0)
		return NULL;
	if (size == 0) {
		free(mem);
		return NULL;
	}
	mem = (t_block *) disalign_memory(mem, ALIGNMENT);
	ptr = (void*)mem - sizeof(t_block);
	if (!is_allocated(ptr) || !is_inside_heap(ptr)) {
		return NULL;
	}
	if (ptr->size != SMALL && ptr->size != TINY && ptr->size < SMALL)
		return NULL;
	if (ptr->size >= size)
		return mem;
	void *new_mem = malloc(size);
	if (!new_mem)
		return NULL;
	ft_memmove(new_mem, mem, ptr->size);
	free(mem);
	return new_mem;


}