#include <ft_malloc.h>

t_block *g_head = NULL;

/***
 * show_alloc_mem
 * This function is used to print the memory allocation of the pre-allocated blocks.
 */
void show_alloc_mem() {
    t_block *tmp = g_head;
    size_t offset;
    unsigned int i = 0;

    printf("TINY : %p\n", (void *) tmp);

    while (tmp != NULL) {
        if (tmp->size == TINY) {
            offset = TINY;
        } else if (tmp->size == SMALL) {
            offset = SMALL;
        } else if (tmp->size == LARGE) {
            offset = LARGE;
        } else 
            offset = 0;
        

        printf("%d · %p - %p : %lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)), 
               (void *)((char *)tmp + sizeof(t_block) + offset), 
               tmp->size);

        tmp = tmp->next;
        ++i;
    }
}

void hexdump(void *ptr, size_t size) {
    unsigned char *p = (unsigned char *)ptr;
    unsigned char c;
    unsigned int i = 0;

    while (i < size) {
        c = p[i];
        printf("%02x ", c);
        ++i;
        if (i % 16 == 0)
            printf("\n");
    }
    printf("\n");
}


/****
 * show_alloc_mem_ex
 * This function shows extra information (an hex dump of allocated memory)
*/
void show_alloc_mem_ex(){
    t_block *tmp = g_head;
    size_t offset;
    unsigned int i = 0;

    printf("TINY : %p\n", (void *) tmp);

    while (tmp != NULL) {
        if (tmp->size == TINY) {
            offset = TINY;
        } else if (tmp->size == SMALL) {
            offset = SMALL;
        } else if (tmp->size == LARGE) {
            offset = LARGE;
        } else 
            offset = 0;
        

        printf("%d · %p - %p : %lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)), 
               (void *)((char *)tmp + sizeof(t_block) + offset), 
               tmp->size);

        hexdump((void *)((char *)tmp + sizeof(t_block)), tmp->size);

        tmp = tmp->next;
        ++i;
    }

}


static size_t calculate_total_memory(){
	return (sizeof(t_block) + TINY) * N_BLOCKS + (sizeof(t_block) + SMALL) * N_BLOCKS + (sizeof(t_block) + LARGE) * N_BLOCKS;
}


static int prealloc(void) {
    int i;

    t_block *tmp = NULL;
    size_t tiny_tblock = sizeof(t_block) + TINY;
    size_t small_tblock = sizeof(t_block) + SMALL;

    if (g_head != NULL)
        return 0;

    g_head = mmap(NULL, calculate_total_memory(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_head == MAP_FAILED)
        return -1;

    // TINY
    tmp = g_head;
    for (i = 0; i < N_BLOCKS; i++) {
        tmp->size = TINY;
        tmp->inuse = false;
        tmp->next = (t_block *)((char *)tmp + tiny_tblock);
        tmp->prev = i > 0 ? (t_block *)((char *)tmp - tiny_tblock) : NULL;
        tmp = tmp->next;
    }
    // SMALL
    for (i = 0; i < N_BLOCKS; i++) {
        tmp->size = SMALL;
        tmp->inuse = false;
        tmp->next = (t_block *)((char *)tmp + small_tblock);
        tmp->prev = i > 0 ? (t_block *)((char *)tmp - small_tblock) : NULL;
        tmp = tmp->next;
    }
    // LARGE
    for (i = 0; i < N_BLOCKS; i++) {
        tmp->size = LARGE;
        tmp->inuse = false;
        tmp->next = i < N_BLOCKS - 1 ? (t_block *)((char *)tmp + sizeof(t_block) + LARGE) : NULL;
        tmp->prev = i > 0 ? (t_block *)((char *)tmp - sizeof(t_block) - LARGE) : NULL;
        tmp = tmp->next;
    }

    // show_alloc_mem();
    // show_alloc_mem_ex();
    int ret = munmap(g_head, calculate_total_memory());
    if (ret == -1)
        return -1;
    return 0;
}


/// @brief well it's malloc!
/// @param size bytes to allocate
/// @return a pointer to memory or NULL in case of failure
void *malloc(size_t size) {
    void *ptr = NULL;
    int page_size = getpagesize();
    struct rlimit limit;
    int ret;

    ret = prealloc();
    if (ret)
        return NULL;
    // printf("page size: %d\n", page_size);
    getrlimit(RLIMIT_AS, &limit);
    if (size > limit.rlim_cur)
        return NULL;
// printf("limit: %lu\n", limit.rlim_cur);
    if (size == 0) {
        size = page_size;
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
        return ptr;
    }
    return NULL;
}

void free(void *ptr) {
    (void) ptr;
    // free(ptr);
}

void *ft_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}