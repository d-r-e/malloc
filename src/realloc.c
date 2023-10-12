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
	t_block *tmp;

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

/// \brief
///  Reallocates a memory block
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
	void *dst;
	void *actual_mem;
	t_block *tmp;

	if (!mem)
		return malloc(size);
	actual_mem = disalign_memory(mem, ALIGNMENT);
	tmp = (t_block *) actual_mem - 1;
	if (!is_allocated(tmp))
		return malloc(size);
	if (tmp->size != SMALL && tmp->size != TINY && tmp->size < SMALL)
		return NULL;
	if (size <= TINY && tmp->size == TINY)
		return mem;
	if (size > TINY && size <= SMALL && tmp->size == SMALL)
		return mem;
	dst = malloc(size);
	if (!dst)
		return NULL;
	if (size < tmp->size)
		ft_memmove(dst, mem, size);
	else
		ft_memmove(dst, mem, tmp->size);
	free(mem);
	return dst;
}