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

void print_tblock_header(t_block block){
    printf("in use: %d |", block.inuse);
    printf("size  : %lu\n", block.size);
}


void print_hex_tblock_body(t_block *block){
    printf("Address: %p\n", (void *)((char *)&block + sizeof(t_block)));
    hexdump((void *)((char *)block + sizeof(t_block)), block->size);
}
