#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_malloc()
{
    int page_size = getpagesize();
    printf("page_size %d\n", page_size);
    char *ptr = malloc(100);
    char *ptr2 = malloc(10);
    // char *ptr3 = malloc(20);

    ft_strncpy(ptr, "Hello", 100);

    ft_strncpy(ptr2, "World", 10);
    printf("%s\n", ptr);
    // free(ptr);
    printf("%s\n", ptr2);
    char *ptr3 = malloc(100);
    if (ptr3 == NULL){
        puts("malloc failed\n");
        exit(-1);
    }
    else {
        puts("malloc succeeded\n");
    }
    strncpy(ptr3, "Thistest", 10);
    free(ptr);
    free(ptr2);
    free(ptr3);
    // printf("%s\n", ptr3);
    // char *ptr4 = malloc(0);
    // (void)ptr4;
    // char *ptr3 = malloc(TINY + 1);
    // ft_strncpy(ptr3, "This is a test", TINY + 1);
    // printf("%s\n", ptr3);
    
}


int main()
{
    test_malloc();
    // test_malloc_zero_size();
    // test_malloc_bigger_than_as_rlimit();
    // test_free();
    // test_free_null();
    // test_ft_realloc();
    // test_ft_realloc_smaller();
    // test_ft_realloc_null();
    return 0;
}
