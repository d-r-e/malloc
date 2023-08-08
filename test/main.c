#include <stdio.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_ft_malloc()
{
    void *ptr = ft_malloc(10);
    if (ptr != NULL)
    {
        for (int i = 0; i < 10; i++)
        {
            ((char *)ptr)[i] = 'B';
        }
        printf(GREEN "[ ✔ ]"RESET);
    }
    else
    {
        printf(RED "[ ✘ ]" RESET);
    }
    ft_free(ptr);
}

void test_ft_free()
{
    void *ptr = ft_malloc(10);
    ft_free(ptr);
    printf(GREEN "[ ✔ ]" RESET );
}

void test_ft_realloc()
{
    void *ptr = ft_malloc(10);
    ptr = ft_realloc(ptr, 20);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET);
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc failed\n");
    }
    ft_free(ptr);
}

void test_ft_malloc_zero_size()
{
    void *ptr = ft_malloc(0);
    ft_free(ptr);
    printf(GREEN "[ ✔ ]" RESET );
}

void test_malloc_bigger_than_as_rlimit()
{
    struct rlimit limit;

    getrlimit(RLIMIT_AS, &limit);
    void *ptr = ft_malloc(limit.rlim_cur + 1);
    if (ptr == NULL)
    {
        printf(GREEN "[ ✔ ]" RESET);
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_malloc with size bigger than RLIMIT_AS (%lu) failed\n", limit.rlim_cur);
    }
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
    test_malloc_bigger_than_as_rlimit();
    // test_ft_free();
    // test_ft_free_null();
    // test_ft_realloc();
    // test_ft_realloc_smaller();
    // test_ft_realloc_null();
    ft_puts("");
    return 0;
}
