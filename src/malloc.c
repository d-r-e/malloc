/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:13:30 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/08 11:14:24 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

t_heap g_heap = {NULL, NULL, NULL};


static void initialize_block(t_block *block, size_t size, size_t tblock_size, int i) {
#ifdef FILL_MEMORY
    char c = 'B' + i;
    while (!ft_isprint(c))
        c++;
    ft_memset((void *)((char *)block + sizeof(t_block)), c, size);
#endif
    block->size = size;
    block->inuse = false;
    if (size <= SMALL) {
        block->next = (t_block * )((char *) block + tblock_size);
        block->prev = (i > 0) ? (t_block * )((char *) block - tblock_size) : NULL;
    } else{}
//        block->next = NULL;

}

static int prealloc(void) {
    int i;
    t_block *tmp = NULL;
    size_t tiny_tblock = sizeof(t_block) + TINY;
    size_t small_tblock = sizeof(t_block) + SMALL;
    size_t large_block = sizeof(t_block) + LARGE + ALIGNMENT;

    if (g_heap.tiny != NULL)
        return 0;
    else {
        g_heap.tiny = mmap(NULL, (sizeof(t_block) + TINY) * N_BLOCKS, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (g_heap.tiny == MAP_FAILED) {
            printf("mmap failed\n");
            return -1;
        }
        // TINY
        tmp = g_heap.tiny;
        for (i = 0; i < N_BLOCKS; i++) {
            initialize_block(tmp, TINY, tiny_tblock, i);
            if (i == N_BLOCKS - 1)
                tmp->next = NULL;
            tmp = tmp->next;
        }
    }
    g_heap.small = mmap(NULL, (sizeof(t_block) + SMALL) * N_BLOCKS, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);
    if (g_heap.small == MAP_FAILED) {
        printf("mmap failed\n");
        return -1;
    }
    tmp = g_heap.small;
    for (i = 0; i < N_BLOCKS; i++) {
        initialize_block(tmp, SMALL, small_tblock, i);
        if (i == N_BLOCKS - 1)
            tmp->next = NULL;
        tmp = tmp->next;
    }
    g_heap.large = mmap(NULL, large_block * 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_heap.large == MAP_FAILED) {
        printf("mmap failed\n");
        return -1;
    }
    initialize_block(g_heap.large, LARGE, large_block, 0);
    g_heap.large->prev = NULL;
    g_heap.large->next = NULL;
    t_block *last_block = g_heap.large;
    for (i = 1; i < N_BLOCKS; i++) {
        tmp = mmap(NULL, large_block * 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (tmp == MAP_FAILED) {
            printf("mmap failed\n");
            return -1;
        }
        initialize_block(tmp, LARGE, sizeof(t_block) + LARGE, i);
        last_block->next = tmp;
        tmp->prev = last_block;
        if (i == N_BLOCKS - 1) {
            tmp->next = NULL;
        }
        last_block = tmp;
    }
    // show_alloc_mem();
    // show_alloc_mem_ex();
    return 0;
}


/// @brief Adds a N_BLOCKS block of size SIZE to the appropriate
///heap zone for SMALL and TINY sizes sets it to mem.next
/// @param mem 
/// @param size 
/// @return 
static int extend_heap(t_block *mem, size_t size) {

    t_block *tmp = NULL;

    size_t size_to_extend = N_BLOCKS * (sizeof(t_block) + size);
    tmp = mmap(NULL, size_to_extend, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (tmp == MAP_FAILED) {
        printf("mmap failed\n");
        return -1;
    }
    mem->next = tmp;
    for (int i = 0; i < N_BLOCKS; i++) {
        initialize_block(tmp, size, size + sizeof(t_block), i);
        if (i == N_BLOCKS - 1)
            tmp->next = NULL;
        tmp = tmp->next;
    }
    return 0;
}

static
void *get_aligned_pointer(void *mem, size_t alignment) {
    return (void *) (((size_t) mem + alignment - 1) & ~(alignment - 1));
}


/// @brief Well it's malloc!
/// @param size bytes to allocate
/// @return a pointer to memory or NULL in case of failure
void *malloc(size_t size) {
    t_block *ptr = NULL;
    struct rlimit limit;
    int ret = 0;

    getrlimit(RLIMIT_AS, &limit);
    if (size > limit.rlim_cur - sizeof(t_block))
        return NULL;
    ret = prealloc();
    if (ret)
        return NULL;
    else if (size && size <= SMALL) {
        ptr = g_heap.tiny;
        int i = 0;
        while (size <= TINY && ptr && ptr->size == TINY) {
            i++;
            if (ptr->inuse == false && size <= TINY) {
                ptr->inuse = true;
                return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
            }
            if (ptr->next == NULL)
                extend_heap(ptr, TINY);
            ptr = ptr->next;
        }
        if (size > TINY && size <= SMALL) {
            ptr = g_heap.small;
            i = 0;
            while (ptr && ptr->size == SMALL && size <= SMALL) {
                i++;
                if (ptr->inuse == false) {
                    ptr->inuse = true;
                    return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
                }
                if (ptr->next == NULL)
                    extend_heap(ptr, SMALL);
                ptr = ptr->next;
            }
        }
    } else {
        unsigned int i = 0;
        ptr = g_heap.large;
        while (ptr) {
            if (ptr->inuse == false && size <= ptr->size) {
                ptr->inuse = true;
                return get_aligned_pointer((void *) ((char *) ptr + sizeof(t_block)), ALIGNMENT);
            }
            ++i;
            if (ptr->next)
                ptr = ptr->next;
            else
                break;
        }
        t_block *new_block = mmap(NULL, size + sizeof(t_block) + ALIGNMENT,
                                  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (new_block == MAP_FAILED) {
            ft_putstr(MUNMAP_ERROR_STRING);
            return NULL;
        }
        new_block->inuse = true;
        new_block->size = size;
        ptr->next = new_block;
        new_block->prev = ptr;
        new_block->next = NULL;
        return get_aligned_pointer((void *) ((char *) new_block + sizeof(t_block)), ALIGNMENT);
    }
    return NULL;
}

