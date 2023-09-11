#include <ft_malloc.h>

t_heap g_heap = {NULL, NULL, NULL};

/***
 * show_alloc_mem
 * This function is used to print the memory allocation of the pre-allocated blocks.
 */
void show_alloc_mem()
{
    t_block *tmp = g_heap.tiny;
    printf("TINY : %p\n", (void *)tmp);
    for (unsigned int i = 0; i < N_BLOCKS && tmp; i++)
    {
        printf("%3d · %p - %p : %3lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + TINY),
               tmp->size);

        tmp = tmp->next;
    }
    tmp = g_heap.small;
    printf("SMALL: %p\n", (void *)tmp);
    for (unsigned int i = 0; i < N_BLOCKS && tmp; i++){
        printf("%3d · %p - %p : %3lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + SMALL),
               tmp->size);

        tmp = tmp->next;
    }
}

size_t calculate_total_memory()
{
    return (sizeof(t_block) + TINY) * N_BLOCKS +
           (sizeof(t_block) + SMALL) * N_BLOCKS +
           (sizeof(t_block) + LARGE) * N_BLOCKS;
}

static char get_next_printable_char(char c)
{
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

static int prealloc(void)
{
    int i;
    t_block *tmp = NULL;
    size_t tiny_tblock = sizeof(t_block) + TINY;
    size_t small_tblock = sizeof(t_block) + SMALL;

    if (g_heap.tiny != NULL)
        return 0;
    g_heap.tiny = mmap(NULL, (sizeof(t_block) + TINY) * N_BLOCKS, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_heap.tiny == MAP_FAILED)
    {
        printf("mmap failed\n");
        return -1;
    }
    printf("g_head %p\n", g_heap.tiny);
    // TINY
    tmp = g_heap.tiny;
    for (i = 0; i < N_BLOCKS; i++)
    {
        initialize_block(tmp, TINY, tiny_tblock, i);
        tmp = tmp->next;
    }
    g_heap.small = mmap(NULL, (sizeof(t_block) + SMALL) * N_BLOCKS, PROT_READ| PROT_WRITE, MAP_PRIVATE |MAP_ANONYMOUS, -1, 0);
    if (g_heap.small == MAP_FAILED)
    {
        printf("mmap failed\n");
        return -1;
    }
    printf("g_heap.small %p\n ", g_heap.small);
    tmp = g_heap.small;
    // Initialize SMALL blocks
    for (i = 0; i < N_BLOCKS; i++)
    {
        initialize_block(tmp, SMALL, small_tblock, i);
        tmp = tmp->next;
    }
    show_alloc_mem();
    // show_alloc_mem_ex();
    return 0;
}

/// @brief Well it's malloc!
/// @param size bytes to allocate
/// @return a pointer to memory or NULL in case of failure
void *malloc(size_t size)
{
    void *ptr = NULL;
    struct rlimit limit;
    int ret = 0;

    ret = prealloc();
    // show_alloc_mem_ex();
    if (ret)
        return NULL;
    getrlimit(RLIMIT_AS, &limit);
    if (size > limit.rlim_cur)
        return NULL;
    else if (size && size <= SMALL)
    {
        t_block *ptr = g_heap.tiny;
        int i = 0;
        while (ptr && ptr->size == TINY)
        {
            i++;
            // print_tblock_header(*ptr);
            if (ptr->inuse == false && size <= TINY)
            {
                ptr->inuse = true;
                printf("FOUND TINY  i: %d %p\n", i, (void *)((char *)ptr + sizeof(t_block)));
                return (void *)((char *)ptr + sizeof(t_block));
            }
            ptr = ptr->next;
        }
        if (size > TINY && size <= SMALL)
        {
            ptr = g_heap.small;
            i = 0;
            while (ptr && ptr->size == SMALL && size <= SMALL)
            {
                i++;
                // print_tblock_header(*ptr);
                if (ptr->inuse == false)
                {
                    ptr->inuse = true;
                    // print_hex_tblock_body(ptr);
                    return (void *)((char *)ptr + sizeof(t_block));
                }
                ptr = ptr->next;
            }
        }
    }
    else
    {
        ptr = mmap(NULL, size + sizeof(t_block),
                   PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr == MAP_FAILED)
        {
            ft_putstr("error: mmap failed.\n");
            return NULL;
        }
        return &((char *)ptr)[sizeof(t_block)];
    }
    return NULL;
}

void *ft_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}
