#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ft_malloc.h>

void fillMemory(char *ptr, size_t size, char pattern)
{
    memset(ptr, pattern, size);
}

int checkMemory(char *ptr, size_t size, char expectedPattern)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (ptr[i] != expectedPattern)
        {
            return 0;
        }
    }
    return 1;
}

void shuffle(int *array, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

int main()
{
    const int NUM_ITERATIONS = 10000;
    const int MAX_BLOCK_SIZE = 4096;

    char *blocks[NUM_ITERATIONS];
    size_t blockSizes[NUM_ITERATIONS];
    char blockPatterns[NUM_ITERATIONS];
    int indices[NUM_ITERATIONS];

    for (int i = 0; i < NUM_ITERATIONS; ++i)
    {
        indices[i] = i;
    }
    shuffle(indices, NUM_ITERATIONS);

    clock_t start = clock();

    for (int i = 0; i < NUM_ITERATIONS; ++i)
    {
        size_t blockSize = 1 + rand() % MAX_BLOCK_SIZE;
        blockSizes[i] = blockSize;

        blocks[i] = malloc(blockSize);
        if (blocks[i] == NULL)
        {
            printf("Failed to allocate memory of size %zu in iteration %d\n", blockSize, i);
            return 1;
        }

        char pattern = 'A' + (i % 26);
        blockPatterns[i] = pattern;

        fillMemory(blocks[i], blockSize, pattern);
    }

    for (int i = 0; i < NUM_ITERATIONS; ++i)
    {
        int index = indices[i];

        if (!checkMemory(blocks[index], blockSizes[index], blockPatterns[index]))
        {
            printf("Memory integrity check failed in iteration %d\n", index);
            return 1;
        }

        free(blocks[index]);
    }

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("All iterations completed successfully!\n");
    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    return 0;
}
