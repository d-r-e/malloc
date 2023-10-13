/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:13:30 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/12 11:55:49 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

t_heap g_heap = {NULL, NULL, NULL};


static void initialize_block(t_block *block, size_t size, size_t tblock_size, int i) {
#ifdef MALLOC_DEBUG
	char c = 'B' + i;
	while (!ft_isprint(c))
		c++;
	ft_memset((void *) ((char *) block + sizeof(t_block)), c, size);
#endif
	block->size = size;
	block->inuse = false;
	if (size <= SMALL) {
		block->next = (t_block *) ((char *) block + tblock_size);
		block->prev = (i > 0) ? (t_block *) ((char *) block - tblock_size) : NULL;
	}
}

static int prealloc(void) {
	unsigned int i;
	t_block *tmp = NULL;
	size_t tiny_tblock = TINY + OVERHEAD;
	size_t small_tblock = SMALL + OVERHEAD;

	if (!g_heap.tiny ) {
		g_heap.tiny = mmap(NULL, TINY_ARENA, PROT_READ | PROT_WRITE,
						   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (g_heap.tiny == MAP_FAILED) {
			ft_putstr_fd("mmap failed\n", 2);
			return -1;
		}
		tmp = g_heap.tiny;
		for (i = 0; i < N_BLOCKS; i++) {
			initialize_block(tmp, TINY, tiny_tblock, i);
			if (i == N_BLOCKS - 1)
				tmp->next = NULL;
			tmp = tmp->next;
		}
	}
	if (!g_heap.small) {
		g_heap.small = mmap(NULL, SMALL_ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
							-1, 0);
		if (g_heap.small == MAP_FAILED) {
			ft_putstr_fd("mmap failed\n", 2);
			return -1;
		}
		tmp = g_heap.small;
		for (i = 0; i < N_BLOCKS; i++) {
			initialize_block(tmp, SMALL, small_tblock, i);
			if (i == N_BLOCKS - 1)
				tmp->next = NULL;
			tmp = tmp->next;
		}
	}

#ifdef MALLOC_DEBUG
	printf("Preallocated tiny %u pages\n", TINY_ARENA / PAGE_SIZE);
	if (TINY_ARENA % PAGE_SIZE)
		printf("Warning: TINY_ARENA is not a multiple of page size\n");
	printf("Preallocated small %u pages\n", SMALL_ARENA / PAGE_SIZE);
	if (SMALL_ARENA % PAGE_SIZE)
		printf("Warning: SMALL_ARENA is not a multiple of page size\n");
#endif
	return 0;
}


static int extend_heap(t_block *mem, size_t size) {

	t_block *tmp = NULL;

	size_t size_to_extend = 0;
	if (size <= TINY)
		size_to_extend = TINY_ARENA;
	else if (size <= SMALL)
		size_to_extend = SMALL_ARENA;
	tmp = mmap(NULL, size_to_extend, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (tmp == MAP_FAILED) {
		printf("mmap failed\n");
		return -1;
	}
	mem->next = tmp;
	for (unsigned int i = 0; i < N_BLOCKS; i++) {
		initialize_block(tmp, size, size + sizeof(t_block), i);
		if (i == N_BLOCKS - 1)
			tmp->next = NULL;
		tmp = tmp->next;
	}
	return 0;
}

static void *get_aligned_pointer(void *mem, size_t alignment) {
	return (void *) (((size_t) mem + alignment - 1) & ~(alignment - 1));
}


void *malloc(size_t size) {
	t_block *ptr = NULL;
	struct rlimit limit;
	int ret = 0;

	getrlimit(RLIMIT_AS, &limit);
	if (size > limit.rlim_cur - OVERHEAD)
		return NULL;
	ret = prealloc();
	if (ret || size > SIZE_MAX)
		return NULL;
	else if (size <= SMALL) {
		ptr = g_heap.tiny;
		while (size <= TINY && ptr && ptr->size == TINY) {
			if (ptr->inuse == false && size <= TINY) {
				ptr->inuse = true;
				return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
			}
			if (ptr->next == NULL) {
				ret = extend_heap(ptr, TINY);
				if (ret)
					return NULL;
			}
			ptr = ptr->next;
		}
		if (size > TINY && size <= SMALL) {
			ptr = g_heap.small;
			while (ptr && ptr->size == SMALL && size <= SMALL) {
				if (ptr->inuse == false) {
					ptr->inuse = true;
					return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
				}
				if (ptr->next == NULL) {
					ret = extend_heap(ptr, SMALL);
					if (ret)
						return NULL;
				}
				ptr = ptr->next;
			}
		}
	} else if (size > SMALL){
		if (g_heap.large)
			ptr = g_heap.large;
		else
			ptr =NULL;
		while (ptr) {
			if (ptr->inuse == false && size <= ptr->size) {
				ptr->inuse = true;
#ifdef MALLOC_DEBUG
				MALLOC_DEBUG_PRINT("malloc: debug: found block: %p\n", &ptr);
#endif
				return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
			}
			if (ptr->next)
				ptr = ptr->next;
			else
				break;
		}
		while(ptr && ptr->next)
			ptr = ptr->next;
		size_t total = size + OVERHEAD;

		t_block *new_block = mmap(NULL, total,
								  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (new_block == MAP_FAILED) {
			ft_putstr(MMAP_ERROR_STRING);
			return NULL;
		}
		new_block->inuse = true;
		new_block->size = size;

		if (ptr){
			ptr->next = new_block;
			new_block->prev = ptr;
		}
		else {
			g_heap.large = new_block;
			g_heap.large->prev = NULL;
			g_heap.large->next = NULL;
		}
		new_block->next = NULL;
#ifdef MALLOC_DEBUG
		printf("malloc found block: %p\n", &new_block);
#endif
		return get_aligned_pointer((void *) ((char *) new_block + sizeof(t_block)), ALIGNMENT);
	}
	return NULL;
}
