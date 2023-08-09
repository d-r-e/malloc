#include <ft_malloc.h>

t_block *g_head = NULL;

/***
 * show_alloc_mem
 * This function is used to print the memory allocation of the preallocated blocks.
*/
void show_alloc_mem(){
	t_block *tmp = NULL;

	tmp = g_head;
	printf("TINY : %p\n", (void*)tmp);
	while (tmp != NULL)
	{
		if (tmp->size == TINY)
		{
			printf("%p - %p : %lu bytes\n", (void*)(tmp + sizeof(t_block)), (void*)(tmp + sizeof(t_block) + TINY), tmp->size);
		} else if (tmp->size == SMALL)
		{
			printf("%p - %p : %lu bytes\n", (void*)(tmp + sizeof(t_block)), (void*)(tmp + sizeof(t_block) + SMALL), tmp->size);
		} else if (tmp->size == LARGE)
		{
			printf("%p - %p : %lu bytes\n", (void*)(tmp + sizeof(t_block)), (void*)(tmp + sizeof(t_block) + LARGE), tmp->size);
		}
		tmp = tmp->next;

	}

	
}

/***
 * prealloc
 * For the three predefined sizes, TINY, SMALL and LARGE, we preallocate 100 blocks of each size.
*/
static int prealloc(void)
{
	int i;
	t_block *tmp = NULL;

	if (g_head != NULL)
		return 0;

	// TINY
	g_head = mmap(NULL, (sizeof(t_block) + TINY) * 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (g_head == MAP_FAILED)
		return -1;
	tmp = g_head;
	for (i = 0; i < 3; i++)
	{
		tmp->size = TINY;
		tmp->inuse = false;
		tmp->next = (t_block *)(tmp + sizeof(t_block) + TINY);
		tmp->prev = i > 0 ? (t_block *)(tmp - sizeof(t_block) - TINY) : NULL;
		if (i == 2)
			tmp->next = NULL;
		tmp = tmp->next;
	}
	show_alloc_mem();
	return 0;
}

void *ft_malloc(size_t size)
{
	void *ptr = NULL;
	int page_size = getpagesize();
	struct rlimit limit;

	prealloc();

// printf("page size: %d\n", page_size);
	getrlimit(RLIMIT_AS, &limit);
	if (size > limit.rlim_cur)
		return NULL;
// printf("limit: %lu\n", limit.rlim_cur);
#ifdef DEBUG
	printf("Malloc'ing %lu bytes 😜 \n", size);
#endif
	if (size == 0)
	{
		size = page_size;
		ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
		if (ptr == MAP_FAILED)
			return NULL;
#ifdef DEBUG
		printf("Correctly allocated %lu bytes 😜 \n", size);
#endif
		return ptr;
	}
	return malloc(size);
}

void ft_free(void *ptr)
{
	free(ptr);
}

void *ft_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}