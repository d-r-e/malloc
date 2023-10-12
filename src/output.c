/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 00:42:42 by darodrig          #+#    #+#             */
/*   Updated: 2023/10/12 11:47:04 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_malloc.h>

static void hexdump(void *ptr, size_t size)
{
    unsigned char *p = (unsigned char *)ptr;
    unsigned char c;
    unsigned int i = 0;

    while (i < size)
    {
        c = p[i];
        printf("%02x ", c);
        ++i;
        if (!(i % 32) && size > 32)
            printf("\n");
        if ((i % 16) && !(i % 32))
            printf("   ");
    }
}


/// @brief Prints the memory blocks allocated in the heap.
void show_alloc_mem()
{
    t_block *tmp = g_heap.tiny;
    unsigned long long total_size = 0;
    
    printf("TINY : %p\n", (void *)tmp);
    for (unsigned int i = 0; i < N_BLOCKS && tmp; ++i)
    {
        printf("%5d · %p - %p : %3lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + TINY),
               tmp->size);
        total_size += tmp->size;
        tmp = tmp->next;
    }
    tmp = g_heap.small;
    printf("SMALL: %p\n", (void *)tmp);
    for(unsigned int i  =0; tmp; ++i)
    {
        printf("%5d · %p - %p : %3lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + SMALL),
               tmp->size);
        tmp = tmp->next;
    }
    tmp = g_heap.large;
    printf("LARGE: %p\n", (void *)tmp);
    int i = 0;
    while (tmp){
        printf("%5d · %p - %p : %3lu bytes\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + tmp->size),
               tmp->size);
        tmp = tmp->next;
        ++i;
    }
}

/// @brief Prints the memory blocks allocated in the heap.
void show_alloc_mem_ex()
{
    t_block *tmp = NULL;
    unsigned int i = 0;

    tmp = g_heap.tiny;
    printf("TINY : %p\n", (void *)tmp);
    while (tmp)
    {
        ft_putstr(RED);
        printf("%5d · %p - %p : %5lu bytes ", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + TINY),
               tmp->size);
        if (tmp->size > 0)
            hexdump((void *)((char *)tmp + sizeof(t_block)), tmp->size);
        tmp = tmp->next;
        printf("\n");
        ++i;
        ft_putstr(RESET);
    }
    tmp = g_heap.small;
    printf("SMALL: %p\n", (void *)tmp);
    while (tmp)
    {
        ft_putstr(PURPLE);
        printf(PURPLE "%d · %p - %p : %5lu BYTES\n", i,
               (void *)((char *)tmp + sizeof(t_block)),
               (void *)((char *)tmp + sizeof(t_block) + SMALL),
               tmp->size);
        if (tmp->size > 0)
            hexdump((void *)((char *)tmp + sizeof(t_block)), tmp->size);
        tmp = tmp->next;
        ++i;
        ft_putstr(RESET);
    }
	tmp = g_heap.large;
	printf("LARGE: %p\n", (void *)tmp);
	while (tmp)
	{
		ft_putstr(GREEN);
		printf("%5d · %p - %p : %5lu bytes ", i,
			   (void *)((char *)tmp + sizeof(t_block)),
			   (void *)((char *)tmp + sizeof(t_block) + tmp->size),
			   tmp->size);
		if (tmp->size > 0)
			hexdump((void *)((char *)tmp + sizeof(t_block)), tmp->size);
		tmp = tmp->next;
		printf("\n");
		++i;
		ft_putstr(RESET);
	}
}

void print_tblock_header(t_block block)
{
    printf("in use: %d |", block.inuse);
    printf("size  : %lu  | ", block.size);
    printf("next  : %p", (void *)block.next);
}

void print_hex_tblock_body(t_block *block)
{
    printf("Address: %p\n", (void *)((char *)&block + sizeof(t_block)));
    hexdump((void *)((char *)block + sizeof(t_block)), block->size);
}
