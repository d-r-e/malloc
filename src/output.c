#include <ft_malloc.h>


static void hexdump(void *ptr, size_t size) {
    unsigned char *p = (unsigned char *)ptr;
    unsigned char c;
    unsigned int i = 0;

    while (i < size) {
        c = p[i];
        printf("%02x ", c);
        ++i;
        if (i % 16 == 0)
            ft_putstr("\n");
    }
    ft_putstr("\n");
}

/****
 * show_alloc_mem_ex
 * This function shows extra information (an hex dump of allocated memory)
*/
void show_alloc_mem_ex(){
    t_block *tmp = g_heap.tiny;
    unsigned int i = 0;

    printf("TINY : %p\n", (void *) tmp);

    while (tmp) {
        printf("%d · %p - %p : %lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)), 
               (void *)((char *)tmp + sizeof(t_block) + TINY), 
               tmp->size);
        hexdump((void *)((char *)tmp + sizeof(t_block)), tmp->size);
        tmp = tmp->next;
        ++i;
    }

}

void print_tblock_header(t_block block){
    printf("in use: %d |", block.inuse);
    printf("size  : %lu  | ", block.size);
    printf("next  : %p", (void*)block.next);
}


void print_hex_tblock_body(t_block *block){
    printf("Address: %p\n", (void *)((char *)&block + sizeof(t_block)));
    hexdump((void *)((char *)block + sizeof(t_block)), block->size);
}
