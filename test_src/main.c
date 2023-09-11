#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_malloc()
{
    // int page_size = getpagesize();
    // printf("page_size %d\n", page_size);
    char *ptr1 = malloc(10);
    char *ptr2;
    
    ptr2= malloc(3);
    char *ptr3 = malloc(10);

    ft_strlcpy(ptr1, "", 10);
    

    ft_strlcpy(ptr2, "TEST", strlen("TEST") + 1);
    printf("%s\n", ptr1);
    printf("%s\n", ptr2);
    // if (ptr3 == NULL){
    //     ft_puts("malloc failed\n");
    //     exit(-1);
    // }
    // else {
    //     ft_puts("malloc succeeded\n");
    // }
    ft_strlcpy(ptr3, "K ON MY D", 10);
    // printf("%s\n", ptr3);
    // free(ptr1);
    // free(ptr2);
    // free(ptr3);
    // system("leaks test_src.out");
    // printf("%s\n", ptr3);
    // char *ptr4 = malloc(0);
    // (void)ptr4;
    // char *ptr3 = malloc(TINY + 1);
    // ft_strncpy(ptr3, "This is a test_src", TINY + 1);
    // printf("%s\n", ptr3);
    
}


int main()
{
    test_malloc();
    return 0;
}
