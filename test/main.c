#include <stdio.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_malloc()
{
    printf("Testing malloc...\n");
    void *ptr = malloc(10);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET " malloc succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " malloc failed\n");
    }
    free(ptr);
}

void test_free()
{
    printf("Testing free...\n");
    void *ptr = malloc(10);
    free(ptr);
    printf(GREEN "[ ✔ ]" RESET " free succeeded\n");
}

void test_realloc()
{
    printf("Testing realloc...\n");
    void *ptr = malloc(10);
    ptr = realloc(ptr, 20);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET " realloc succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " realloc failed\n");
    }
    free(ptr);
}

int main()
{
    // test_malloc();
    // test_free();
    // test_realloc();
    return 0;
}
