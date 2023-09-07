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


static size_t calculate_total_memory(){
	return (sizeof(t_block) + TINY) * N_BLOCKS + \
    (sizeof(t_block) + SMALL) * N_BLOCKS + \
     (sizeof(t_block) + LARGE) * N_BLOCKS;
}

static char get_next_printable_char(char c){
    while (!ft_isprint(c))
        c++;
    return c;
}

void initialize_block(t_block *block, size_t size, size_t tblock_size, int i)
{
    block->size = size;
    block->inuse = false;
    block->next = (t_block *)((char *)block + tblock_size);
    block->prev = i > 0 ? (t_block *)((char *)block - tblock_size) : NULL;
    ft_memset((void *)((char *)block + sizeof(t_block)), get_next_printable_char('A' + i), size);
}

static int prealloc(void) {
    int i;
    t_block *tmp = NULL;
    size_t tiny_tblock = sizeof(t_block) + TINY;
    size_t small_tblock = sizeof(t_block) + SMALL;
    size_t total_memory;

    if (g_head != NULL)
        return 0;
    total_memory = calculate_total_memory();
    g_head = mmap(NULL, total_memory, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_head == MAP_FAILED)
        return -1;

    // TINY
    tmp = g_head;
    for (i = 0; i < N_BLOCKS; i++)
    {
        initialize_block(tmp, TINY, tiny_tblock, i);
        tmp = tmp->next;
    }

    // Initialize SMALL blocks
    for (i = 0; i < N_BLOCKS; i++)
    {
        initialize_block(tmp, SMALL, small_tblock, i);
        tmp = tmp->next;
    }
    // LARGE
    // for (i = 0; i < N_BLOCKS; i++) {
    //     tmp->size = LARGE;
    //     tmp->inuse = false;
    //     tmp->next = i < N_BLOCKS - 1 ? (t_block *)((char *)tmp + sizeof(t_block) + LARGE) : NULL;
    //     tmp->prev = i > 0 ? (t_block *)((char *)tmp - sizeof(t_block) - LARGE) : NULL;
    //     tmp = tmp->next;
    // }

    // show_alloc_mem();
    // show_alloc_mem_ex();
    // int ret = munmap(g_head, calculate_total_memory());
    // if (ret == -1)
    //     return -1;
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
    show_alloc_mem_ex();
    if (ret)
        return NULL;
    getrlimit(RLIMIT_AS, &limit);
    if (size > limit.rlim_cur)
        return NULL;
    if (size == 0) {
        size = page_size;
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
        return ptr;
    }
    else if (size <= SMALL){
        t_block *ptr = g_head;
        int i = 0;
        while (ptr && ptr->size == TINY)
        {
            i++;
            print_tblock_header(*ptr);
            if (ptr->inuse == false && size <= TINY){
                ptr->inuse = true;
                printf("FOUND TINY i: %d %p\n", i, (void *)((char *)ptr + sizeof(t_block)));
                return (void *)((char *)ptr + sizeof(t_block));
            }
            ptr = ptr->next;
        }
        while (ptr && ptr->size == SMALL && size <= SMALL)
        {
            i++;
            print_tblock_header(*ptr);
            if (ptr->inuse == false){
                ptr->inuse = true;
                printf("FOUND SMALL i: %d\n", i);
                print_hex_tblock_body(ptr);
                return (void *)((char *)ptr + sizeof(t_block));
            }
            ptr = ptr->next;
        }
    }
    else{
        ptr = mmap(NULL, size + sizeof(t_block), PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
        return ptr;
    }
    return NULL;
}


void *ft_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}
