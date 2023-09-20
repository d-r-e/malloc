#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_malloc()
{
    char *ptr1 = malloc(SMALL);
    ft_strlcpy(ptr1, "", 10);

    {
        const size_t num_iter = 100;
        char *ptrs[num_iter];


        for (size_t i = 0; i < num_iter; ++i)
        {
            ptrs[i] = malloc(SMALL);
            ft_strlcpy(ptrs[i], "TEST", strlen("TEST") + 1);
            if (!ft_strncmp(ptrs[i], "TEST", strlen("TEST") + 1))
                printf(GREEN "OK\n" RESET);
            else
                printf(RED "KO\n" RESET);
            
        }
        for (size_t i = 0; i < num_iter; ++i)
        {
            free(ptrs[i]);
        }
    }

    // char *ptr2 = malloc(TINY);
    // ft_strlcpy(ptr2, "TEST", strlen("TEST") + 1);
    
    
    // char *ptr3 = malloc(32);
    // ft_strlcpy(ptr3, "K ON MY D", 10);
    
}


int main()
{
    test_malloc();
    return 0;
}
