#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libft.h>
#include <time.h>
#include <ft_malloc.h>

#define MIN_ITER 2000
#define MAX_ITER 500000
#define MIN_SIZE 1
#define MAX_SIZE 999999
#define LARGE_SIZE 100000000
#define TEST_CASES 1000
#define MAX_POINTERS 100

void seed_rand()
{
    srand(time(NULL));
}

size_t get_random_number(size_t min, size_t max)
{
    return (rand() % (max - min + 1)) + min;
}

char *generate_random_string(size_t length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *string = malloc(length + 1);

    for (size_t i = 0; i < length; i++)
    {
        string[i] = charset[get_random_number(0, sizeof(charset) - 2)];
    }

    string[length] = '\0';
    printf("Generated string: %s\n", string);
    return string;
}

void test_malloc_small()
{
    printf("Testing small malloc...\n");

    char *ptr = malloc(10);
    if (ptr == NULL)
    {
        printf(RED "Failed to allocate small memory!\n" RESET);
        return;
    }

    strcpy(ptr, "test");
    if (strcmp(ptr, "test") == 0)
    {
        printf(GREEN "Small malloc OK\n" RESET);
    }
    else
    {
        printf(RED "Small malloc failed: expected 'test', got '%s'\n" RESET, ptr);
    }

    free(ptr);
}

void test_malloc_large()
{
    printf("Testing large malloc...\n");

    char *ptr = malloc(10 * 1024 * 1024); // 10 MB
    if (ptr == NULL)
    {
        printf(RED "Failed to allocate large memory!\n" RESET);
        return;
    }

    for (int i = 0; i < 10 * 1024 * 1024; i++)
    {
        ptr[i] = i % 256; // just to fill the memory
    }

    printf(GREEN "Large malloc OK\n" RESET);
    free(ptr);
}

void test_double_array()
{
    printf("Testing double array...\n");

    int rows = 100;
    int cols = 100;
    int **array = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        array[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
        {
            array[i][j] = i * j;
        }
    }

    int success = 1;
    for (int i = 0; i < rows && success; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (array[i][j] != i * j)
            {
                printf(RED "Double array failed at [%d][%d]: expected %d, got %d\n" RESET, i, j, i * j, array[i][j]);
                success = 0;
                break;
            }
        }
    }

    if (success)
    {
        printf(GREEN "Double array OK\n" RESET);
    }

    for (int i = 0; i < rows; i++)
    {
        free(array[i]);
    }
    free(array);
}

void test_malloc()
{
    seed_rand();

    const size_t num_iter = get_random_number(MIN_ITER, MAX_ITER);
    char *ptrs[num_iter];
    char *test_strings[num_iter]; // Store the test strings for later verification.

    for (size_t i = 0; i < num_iter; ++i)
    {
        size_t length = get_random_number(MIN_SIZE, MAX_SIZE);
        char *test_string = generate_random_string(length);

        ptrs[i] = malloc(length + 1);
        ft_strlcpy(ptrs[i], test_string, length + 1);
        test_strings[i] = test_string; // Store the test string.
    }

    // Verify in reverse order.
    for (ssize_t i = num_iter - 1; i >= 0; --i)
    {
        if (!ft_strncmp(ptrs[i], test_strings[i], strlen(test_strings[i]) + 1))
        {
            printf(GREEN "OK\n" RESET);
        }
        else
        {
            printf(RED "KO\n" RESET);
        }
    }

    // Clean up: free all memory.
    for (size_t i = 0; i < num_iter; ++i)
    {
        free(test_strings[i]);
        free(ptrs[i]);
    }
}

void test_integer_array()
{
    printf("Testing integer array...\n");

    int size = 1000;
    int *integers = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        integers[i] = i;
    }

    int success = 1;
    for (int i = 0; i < size; i++)
    {
        if (integers[i] != i)
        {
            printf(RED "Integer array failed at %d: expected %d, got %d\n" RESET, i, i, integers[i]);
            success = 0;
            break;
        }
    }

    if (success)
    {
        printf(GREEN "Integer array OK\n" RESET);
    }

    free(integers);
}

void test_malloc_with_double_arr_integers()
{
    int **ptrs = malloc(sizeof(int *) * 10);
    for (int i = 0; i < 10; ++i)
    {
        ptrs[i] = malloc(sizeof(int) * 10);
    }

    for (int i = 0; i < 10; ++i)
    {
        free(ptrs[i]);
    }
    free(ptrs);
}

void test_large_memory_copy()
{
    printf("Testing large memory copy with different characters...\n");

    unsigned char *src = malloc(LARGE_SIZE);
    unsigned char *dest = malloc(LARGE_SIZE);

    if (src == dest)
    {
        printf(RED "Memory allocation addresses are the same in large memory copy test.\n" RESET);
        return;
    }

    if (!src || !dest)
    {
        printf(RED "Memory allocation failure in large memory copy test.\n" RESET);
        return;
    }

    for (int i = 0; i < LARGE_SIZE; i++)
    {
        src[i] = i % 256; // This will loop through all 256 ASCII characters, including non-printables
    }

    ft_memcpy(dest, src, LARGE_SIZE);

    for (int i = 0; i < LARGE_SIZE; i++)
    {
        if (dest[i] != i % 256)
        {
            printf(RED "Mismatch at index %d: expected %d, got %d\n" RESET, i, i % 256, dest[i]);
            free(src);
            free(dest);
            return;
        }
    }

    printf(GREEN "Large memory copy with different characters OK\n" RESET);
    free(src);
    free(dest);
}

void test_memory_overwrite()
{
    printf("Testing memory overwrite...\n");

    int num_pointers = 1000;
    unsigned char *pointers[num_pointers];
    int length = 100;

    for (int i = 0; i < num_pointers; i++)
    {
        pointers[i] = malloc(length);
        if (!pointers[i])
        {
            printf(RED "Memory allocation failure in memory overwrite test.\n" RESET);
            return;
        }
        for (int j = 0; j < length; j++)
        {
            pointers[i][j] = i % 256;
        }
    }

    for (int i = 0; i < num_pointers; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (pointers[i][j] != i % 256)
            {
                printf(RED "Memory overwrite detected at pointer %d index %d: expected %d, got %d\n" RESET, i, j, i % 256, pointers[i][j]);
                for (int k = 0; k < num_pointers; k++)
                {
                    free(pointers[k]);
                }
                return;
            }
        }
    }

    printf(GREEN "Memory overwrite test OK\n" RESET);
    for (int i = 0; i < num_pointers; i++)
    {
        free(pointers[i]);
    }
}

void test_negative(){
    // as malloc prototype uses size_t, neg values will overflow as big integers and allocate a large chung
    char *ptr;
    size_t negative = 0;
    int error = 0;

    printf("Testing negative value %lu...\n", negative);
    for (int i = 0; i< 10; i++){
        negative = -1 * get_random_number(1, 10000);
        ptr = malloc(negative);
        if (ptr != NULL){
            free(ptr);
        } 
    }
    if (error == 0){
        printf(GREEN "Malloc negative test OK\n" RESET);
    }

}

void test_zero(){
    char *ptr;

    printf("Testing zero values...\n");

    ptr = malloc(0);
    if (ptr != NULL){
        free(ptr);
        printf(GREEN "Malloc error 0 OK\n" RESET);
    } else {
        printf(RED "Malloc error 0 KO\n" RESET);
    }
}

void test_mem_alignment(void){
    const unsigned int alignment = 8;
    char *ptr_1;
    char *ptr_2;
    char *ptr_3;
    char *ptr_4;

    printf("Testing alignment...\n");

    seed_rand();
    const unsigned i = get_random_number(1, 100);
    const unsigned j = get_random_number(1, 100);
    const unsigned k = get_random_number(1, 100);
    const unsigned l = get_random_number(1, 100);

    ptr_1 = malloc(i);
    ptr_2 = malloc(j);
    ptr_3 = malloc(k);
    ptr_4 = malloc(l);

    if ((uintptr_t)ptr_1 % alignment)
        printf(RED "Alignment error i (%u) KO\n" RESET, i);
    if ((uintptr_t)ptr_2 % alignment)
        printf(RED "Alignment error j (%u) KO\n" RESET, j);
    if ((uintptr_t)ptr_3 % alignment)
        printf(RED "Alignment error k (%u) KO\n" RESET, k);
    if ((uintptr_t)ptr_4 % alignment)
        printf(RED "Alignment error k (%u) KO\n" RESET, l);
    free(ptr_1);
    free(ptr_2);
    free(ptr_3);
    free(ptr_4);
    printf(GREEN "Alignment test OK\n" RESET);
}


void test_mem_alignment_extensive(void)
{
    const unsigned int alignment = 8;
    char *ptrs[MAX_POINTERS];
    unsigned sizes[MAX_POINTERS];
    int error_found;
    
    error_found = 0;
    printf("Testing alignment extensively...\n");
    seed_rand();
    for (unsigned test = 0; test < TEST_CASES; test++)
    {
        for (unsigned p = 0; p < MAX_POINTERS; p++)
        {
            sizes[p] = get_random_number(1, 100000);
            ptrs[p] = malloc(sizes[p]);

            if (!ptrs[p])
            {
                printf(RED "Memory allocation error for size %u\n" RESET, sizes[p]);
                exit(1);
            }
            if ((uintptr_t)ptrs[p] % alignment)
            {
                printf(RED "Alignment error in test %u for size (%u) KO\n" RESET, test, sizes[p]);
                error_found = 1;
            }
            free(ptrs[p]);
        }
    }
    if (!error_found) {
        printf(GREEN "All %d tests passed!\n" RESET, TEST_CASES);
    }
}

int main()
{
    test_malloc_small();
    test_malloc_large();
    test_double_array();
    test_integer_array();
    test_large_memory_copy();
    test_memory_overwrite();
    // test_negative();
    test_zero();
    test_mem_alignment();
    test_mem_alignment_extensive();
    test_malloc_with_double_arr_integers();
    // show_alloc_mem();
}
