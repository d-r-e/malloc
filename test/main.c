#include <stdio.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_ft_malloc()
{
    printf("Testing ft_malloc...\n");
    void *ptr = ft_malloc(10);
    if (ptr != NULL)
    {
        for (int i = 0; i < 10; i++)
        {
            ((char *)ptr)[i] = 'B';
        }
        printf(GREEN "[ ✔ ]" RESET " ft_malloc succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_malloc failed\n");
    }
    ft_free(ptr);
}

void test_ft_free()
{
    printf("Testing ft_free...\n");
    void *ptr = ft_malloc(10);
    ft_free(ptr);
    printf(GREEN "[ ✔ ]" RESET " ft_free succeeded\n");
}

void test_ft_realloc()
{
    printf("Testing ft_realloc...\n");
    void *ptr = ft_malloc(10);
    ptr = ft_realloc(ptr, 20);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET " ft_realloc succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc failed\n");
    }
    ft_free(ptr);
}

#include <stdio.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_ft_malloc_zero_size()
{
    printf("Testing ft_malloc with zero size...\n");
    void *ptr = ft_malloc(0);
    ft_free(ptr);
    printf(GREEN "[ ✔ ]" RESET " ft_malloc with zero size succeeded\n");
}

void test_ft_free_null()
{
    printf("Testing ft_free with NULL pointer...\n");
    ft_free(NULL); // Should not crash
    printf(GREEN "[ ✔ ]" RESET " ft_free with NULL succeeded\n");
}

void test_ft_realloc_smaller()
{
    printf("Testing ft_realloc with smaller size...\n");
    void *ptr = ft_malloc(20);
    ptr = ft_realloc(ptr, 10);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET " ft_realloc with smaller size succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc with smaller size failed\n");
    }
    ft_free(ptr);
}


void test_ft_realloc_null()
{
    printf("Testing ft_realloc with NULL pointer...\n");
    void *ptr = ft_realloc(NULL, 10);
    if (ptr != NULL) // Depending on the implementation, you might expect this to act like malloc
    {
        printf(GREEN "[ ✔ ]" RESET " ft_realloc with NULL pointer succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc with NULL pointer failed\n");
    }
    ft_free(ptr);
}

int main()
{
    test_ft_malloc();
    test_ft_malloc_zero_size();
    // test_ft_free();
    // test_ft_free_null();
    // test_ft_realloc();
    // test_ft_realloc_smaller();
    // test_ft_realloc_null();
    return 0;
}
