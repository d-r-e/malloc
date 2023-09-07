#include <stdio.h>
#include <ft_malloc.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_malloc()
{
    char *ptr = malloc(10);
    char *ptr2 = malloc(10);
    // char *ptr3 = malloc(20);

    (void)ptr;
    write(1, &ptr, 10);
    puts("");
    strncpy(ptr, "Hello", 10);

    ft_strncpy(ptr2, "World", 10);
    // ft_strncpy(ptr3, "!", 10);
    printf("%s\n", ptr);
    // free(ptr);
    printf("%s\n", ptr2);
    char *ptr3 = malloc(10);
    ft_strncpy(ptr3, "Thistest", 10);
    printf("%s\n", ptr3);
    // char *ptr3 = malloc(TINY + 1);
    // ft_strncpy(ptr3, "This is a test", TINY + 1);
    // printf("%s\n", ptr3);
    
}

void test_free()
{
    void *ptr = malloc(10);
    free(ptr);
    printf(GREEN "[ ✔ ]" RESET );
}

void test_ft_realloc()
{
    void *ptr = malloc(10);
    ptr = ft_realloc(ptr, 20);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET);
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc failed\n");
    }
    free(ptr);
}

void test_malloc_zero_size()
{
    void *ptr = malloc(0);
    free(ptr);
    printf(GREEN "[ ✔ ]" RESET );
}

void test_malloc_bigger_than_as_rlimit()
{
    struct rlimit limit;

    getrlimit(RLIMIT_AS, &limit);
    void *ptr = malloc(limit.rlim_cur + 1);
    if (ptr == NULL)
    {
        printf(GREEN "[ ✔ ]" RESET);
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " malloc with size bigger than RLIMIT_AS failed\n");
    }
}

void test_free_null()
{
    printf("Testing free with NULL pointer...\n");
    free(NULL); // Should not crash
    printf(GREEN "[ ✔ ]" RESET " free with NULL succeeded\n");
}

void test_ft_realloc_smaller()
{
    printf("Testing ft_realloc with smaller size...\n");
    void *ptr = malloc(20);
    ptr = ft_realloc(ptr, 10);
    if (ptr != NULL)
    {
        printf(GREEN "[ ✔ ]" RESET " ft_realloc with smaller size succeeded\n");
    }
    else
    {
        printf(RED "[ ✘ ]" RESET " ft_realloc with smaller size failed\n");
    }
    free(ptr);
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
    free(ptr);
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
    ft_puts("");
    return 0;
}
