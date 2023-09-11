#include <ft_malloc.h>

t_heap g_heap = {NULL, NULL, NULL};



static char get_next_printable_char(char c)
{
    while (!ft_isprint(c))
        c++;
    return c;
}

static void initialize_block(t_block *block, size_t size, size_t tblock_size, int i)
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


static int extend_heap(t_block *mem){
    (void)mem;
    return(0);
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
            extend_heap(ptr);
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
