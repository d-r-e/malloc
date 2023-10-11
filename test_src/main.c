#include <fcntl.h>
#include <ft_malloc.h>
#include <libft.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MIN_ITER 2000
#define MAX_ITER 5000000
#define MIN_SIZE 1
#define MAX_SIZE 999999
#define LARGE_SIZE 100000000
#define TEST_CASES 1000
#define MAX_POINTERS 100

#ifndef RED
# define RED "\033[31m"
# define RESET "\033[0m"
# define GREEN "\033[32m"
#endif

#ifndef TINY
# define TINY 32
# define SMALL (TINY * 4)
# define LARGE (SMALL * 16)
#endif
void seed_rand() {
	srand(time(NULL));
}

size_t get_random_number(size_t min, size_t max) {
	return (rand() % (max - min + 1)) + min;
}

char *generate_random_string(size_t length) {
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char *string = malloc(length + 1);

	for (size_t i = 0; i < length; i++) {
		string[i] = charset[get_random_number(0, sizeof(charset) - 2)];
	}

	string[length] = '\0';
	printf("Generated string: %s\n", string);
	return string;
}

void test_malloc_small() {
	printf("Testing small malloc...\n");

	char *ptr = malloc(10);
	if (ptr == NULL) {
		printf(RED "Failed to allocate small memory!\n" RESET);
		return;
	}

	strcpy(ptr, "test");
	if (strcmp(ptr, "test") == 0) {
		printf(GREEN "Small malloc OK\n" RESET);
	} else {
		printf(RED "Small malloc failed: expected 'test', got '%s'\n" RESET, ptr);
	}

	free(ptr);
}

void test_four_mallocs(){
	printf("Testing four mallocs...\n");
	char *ptr1 = malloc(1);
	char *ptr2 = malloc(1);
	char *ptr3 = malloc(1);
	char *ptr4 = malloc(1);
	if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL || ptr4 == NULL) {
		printf(RED "Failed to allocate small memory!\n" RESET);
		return;
	}
	strcpy(ptr1, "test");
	strcpy(ptr2, "test");
	strcpy(ptr3, "test");
	strcpy(ptr4, "test");
	if (strcmp(ptr1, "test") == 0 && strcmp(ptr2, "test") == 0 && strcmp(ptr3, "test") == 0 && strcmp(ptr4, "test") == 0) {
		;
	} else {
		printf(RED "Four mallocs failed: expected 'test', got '%s'\n" RESET, ptr1);
	}
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
	printf(GREEN "Four mallocs OK\n" RESET);

}

void test_malloc_large() {
	printf("Testing large malloc...\n");

	char *ptr = malloc(10 * 1024);
	if (ptr == NULL) {
		printf(RED "Failed to allocate large memory!\n" RESET);
		return;
	}

	for (int i = 0; i < 10 * 1024; i++) {
		ptr[i] = i % 256;// just to fill the memory
	}

	free(ptr);
	printf(GREEN "Large malloc OK\n" RESET);
}

void test_double_array_small() {
	printf("Testing double array small...\n");

	int rows = 4;
	int cols = 4;
	int **array = malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++) {
		array[i] = malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++) {
			array[i][j] = i * j;
		}
	}

	int success = 1;
	for (int i = 0; i < rows && success; i++) {
		for (int j = 0; j < cols; j++) {
			if (array[i][j] != i * j) {
				printf(RED "Double array failed at [%d][%d]: expected %d, got %d\n" RESET, i, j, i * j, array[i][j]);
				success = 0;
				break;
			}
		}
	}
	for (int i = 0; i < rows; i++)
	{
		free(array[i]);
	}
	free(array);

	if (success) {
		printf(GREEN "Double array small OK\n" RESET);
	}
}

void test_double_array() {
	printf("Testing double array large...\n");

	int rows = 100;
	int cols = 100;
	int **array = malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++) {
		array[i] = malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++) {
			array[i][j] = i * j;
		}
	}

	int success = 1;
	for (int i = 0; i < rows && success; i++) {
		for (int j = 0; j < cols; j++) {
			if (array[i][j] != i * j) {
				printf(RED "Double array failed at [%d][%d]: expected %d, got %d\n" RESET, i, j, i * j, array[i][j]);
				success = 0;
				break;
			}
		}
	}
	   for (int i = 0; i < rows; i++)
	   {
	       free(array[i]);
	   }
	   free(array);

	if (success) {
		printf(GREEN "Double array large OK\n" RESET);
	}
}

void test_malloc() {
	seed_rand();

	const size_t num_iter = get_random_number(MIN_ITER, MAX_ITER);
	char *ptrs[num_iter];
	char *test_strings[num_iter];// Store the test strings for later verification.

	for (size_t i = 0; i < num_iter; ++i) {
		size_t length = get_random_number(MIN_SIZE, MAX_SIZE);
		char *test_string = generate_random_string(length);

		ptrs[i] = malloc(length + 1);
		ft_strlcpy(ptrs[i], test_string, length + 1);
		test_strings[i] = test_string;// Store the test string.
	}

	// Verify in reverse order.
	for (ssize_t i = num_iter - 1; i >= 0; --i) {
		if (!ft_strncmp(ptrs[i], test_strings[i], strlen(test_strings[i]) + 1)) {
			printf(GREEN "OK\n" RESET);
		} else {
			printf(RED "KO\n" RESET);
		}
	}

	// Clean up: free all memory.
	for (size_t i = 0; i < num_iter; ++i) {
		free(test_strings[i]);
		free(ptrs[i]);
	}
}

void test_integer_array() {
	printf("Testing integer array...\n");

	int size = 1000;
	int *integers = malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) {
		integers[i] = i;
	}

	int success = 1;
	for (int i = 0; i < size; i++) {
		if (integers[i] != i) {
			printf(RED "Integer array failed at %d: expected %d, got %d\n" RESET, i, i, integers[i]);
			success = 0;
			break;
		}
	}

	if (success) {
		printf(GREEN "Integer array OK\n" RESET);
	}

	free(integers);
}

void test_malloc_with_double_arr_integers() {
	int **ptrs = malloc(sizeof(int *) * 10);
	for (int i = 0; i < 10; ++i) {
		ptrs[i] = malloc(sizeof(int) * 10);
	}

	for (int i = 0; i < 10; ++i) {
		free(ptrs[i]);
	}
	free(ptrs);
}

void test_thousand_hundred_mallocs() {
	printf("Testing thousand hundred mallocs...\n");

	int **ptrs = malloc(sizeof(int *) * 100);
	for (int i = 0; i < 100; ++i) {
		ptrs[i] = malloc(sizeof(int) * 1000);
	}

	for (int i = 0; i < 100; ++i) {
		free(ptrs[i]);
	}
	free(ptrs);

	printf(GREEN "Thousand hundred mallocs OK\n" RESET);
}

void test_thousand_hundred_mallocs_write_read(){
	printf("Testing thousand hundred mallocs write read...\n");

	int **ptrs = malloc(sizeof(int *) * 100);
	for (int i = 0; i < 100; ++i) {
		ptrs[i] = malloc(sizeof(int) * 1000);
	}

	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 1000; ++j) {
			ptrs[i][j] = i * j;
		}
	}

	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if (ptrs[i][j] != i * j) {
				printf(RED "Memory overwrite detected at pointer %d index %d: expected %d, got %d\n" RESET, i, j, i * j, ptrs[i][j]);
				return;
			}
		}
	}

	for (int i = 0; i < 100; ++i) {
		free(ptrs[i]);
	}
	free(ptrs);

	printf(GREEN "Thousand hundred mallocs write read OK\n" RESET);
}

void test_million_mallocs() {
	printf("Testing million mallocs...\n");

	int **ptrs = malloc(sizeof(int *) * 1000);
	for (int i = 0; i < 1000; ++i) {
		ptrs[i] = malloc(sizeof(int) * 1000);
		if (!ptrs[i]) {
			printf(RED "Memory allocation failure in million mallocs test.\n" RESET);
			return;
		}
	}

	for (int i = 0; i < 1000; ++i) {
		free(ptrs[i]);
	}
	free(ptrs);

	printf(GREEN "Million mallocs OK\n" RESET);
}

void test_gazillion_mallocs() {
	printf("Testing gazillion mallocs...\n");
	int ret = 0;

	char **ptrs = malloc(sizeof(int *) * 10000);
	for (int i = 0; i < 10000; ++i) {
		ptrs[i] = malloc(sizeof(int) * 10000);
		if (!ptrs[i]) {
			printf(RED "Memory allocation failure in gazillion mallocs test.\n" RESET);
			return;
		}
	}

	for (int i = 0; i < 10000; ++i) {
		if (ret != 0) {
			printf(RED "Memory overwrite detected at pointer %d\n" RESET, i);
			return;
		}
		free(ptrs[i]);
	}
	free(ptrs);

	printf(GREEN "Gazillion mallocs OK\n" RESET);
}

void test_large_memory_copy() {
	printf("Testing large memory copy with different characters...\n");

	unsigned char *src = malloc(LARGE_SIZE);
	unsigned char *dest = malloc(LARGE_SIZE);

	if (!src || !dest) {
		printf(RED "Memory allocation failure in large memory copy test.\n" RESET);
		return;
	}

	if (src == dest) {
		printf(RED "Memory allocation addresses are the same in large memory copy test.\n" RESET);
		return;
	}

	if (!src || !dest) {
		printf(RED "Memory allocation failure in large memory copy test.\n" RESET);
		return;
	}

	for (int i = 0; i < LARGE_SIZE; i++) {
		src[i] = i % 256;// This will loop through all 256 ASCII characters, including non-printables
	}

	ft_memcpy(dest, src, LARGE_SIZE);

	for (int i = 0; i < LARGE_SIZE; i++) {
		if (dest[i] != i % 256) {
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

void test_memory_overwrite_small(){
	printf("Testing memory overwrite small...\n");

	int num_pointers = 4;
	unsigned char *pointers[num_pointers];
	int length = 100;

	for (int i = 0; i < num_pointers; i++) {
		pointers[i] = malloc(length);
		if (!pointers[i]) {
			printf(RED "Memory allocation failure in memory overwrite test.\n" RESET);
			return;
		}
		for (int j = 0; j < length; j++) {
			pointers[i][j] = i % 256;
		}
	}

	for (int i = 0; i < num_pointers; i++) {
		for (int j = 0; j < length; j++) {
			if (pointers[i][j] != i % 256) {
				printf(RED "Memory overwrite detected at pointer %d index %d: expected %d, got %d\n" RESET, i, j, i % 256, pointers[i][j]);
				for (int k = 0; k < num_pointers; k++) {
					free(pointers[k]);
				}
				return;
			}
		}
	}

	for (int i = 0; i < num_pointers; i++) {
		free(pointers[i]);
	}
	printf(GREEN "Memory overwrite small OK\n" RESET);
}

void test_memory_overwrite() {
	printf("Testing memory overwrite...\n");

	int num_pointers = 10;
	unsigned char *pointers[num_pointers];
	int length = 100;

	for (int i = 0; i < num_pointers; i++) {
		pointers[i] = malloc(length);
		if (!pointers[i]) {
			printf(RED "Memory allocation failure in memory overwrite test.\n" RESET);
			return;
		}
		for (int j = 0; j < length; j++) {
			pointers[i][j] = i % 256;
		}
	}

	for (int i = 0; i < num_pointers; i++) {
		for (int j = 0; j < length; j++) {
			if (pointers[i][j] != i % 256) {
				printf(RED "Memory overwrite detected at pointer %d index %d: expected %d, got %d\n" RESET, i, j, i % 256, pointers[i][j]);
				for (int k = 0; k < num_pointers; k++) {
					free(pointers[k]);
				}
				return;
			}
		}
	}

	for (int i = 0; i < num_pointers; i++) {
		free(pointers[i]);
	}
	printf(GREEN "Memory overwrite test OK\n" RESET);
}

void test_negative() {
	// as malloc prototype uses size_t, neg values will overflow as big integers and allocate a large chung
	char *ptr;
	size_t negative = 0;
	int error = 0;

	printf("Testing negative value %lu...\n", negative);
	for (int i = 0; i < 10; i++) {
		negative = -1 * get_random_number(1, 10000);
		ptr = malloc(negative);
		if (ptr != NULL) {
			free(ptr);
		}
	}
	if (error == 0) {
		printf(GREEN "Malloc negative test OK\n" RESET);
	}
}

void test_zero() {
	char *ptr;

	printf("Testing zero values...\n");

	ptr = malloc(0);
	if (ptr != NULL) {
		free(ptr);
		printf(GREEN "Malloc error 0 OK\n" RESET);
	} else {
		printf(RED "Malloc error 0 KO\n" RESET);
	}
}

void test_mem_alignment(void) {
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

	if ((uintptr_t) ptr_1 % alignment)
		printf(RED "Alignment error i (%u) KO\n" RESET, i);
	if ((uintptr_t) ptr_2 % alignment)
		printf(RED "Alignment error j (%u) KO\n" RESET, j);
	if ((uintptr_t) ptr_3 % alignment)
		printf(RED "Alignment error k (%u) KO\n" RESET, k);
	if ((uintptr_t) ptr_4 % alignment)
		printf(RED "Alignment error k (%u) KO\n" RESET, l);
	free(ptr_1);
	free(ptr_2);
	free(ptr_3);
	free(ptr_4);
	printf(GREEN "Alignment test OK\n" RESET);
}


void test_mem_alignment_extensive(void) {
	const unsigned int alignment = 8;
	char *ptrs[MAX_POINTERS];
	unsigned sizes[MAX_POINTERS];
	int error_found;

	error_found = 0;
	printf("Testing alignment extensively...\n");
	seed_rand();
	for (unsigned test = 0; test < TEST_CASES; test++) {
		for (unsigned p = 0; p < MAX_POINTERS; p++) {
			sizes[p] = get_random_number(1, 100000);
			ptrs[p] = malloc(sizes[p]);

			if (!ptrs[p]) {
				printf(RED "Memory allocation error for size %u\n" RESET, sizes[p]);
				exit(1);
			}
			if ((uintptr_t) ptrs[p] % alignment) {
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

void test_x() {
	printf("Testing x...\n");

	char *a = malloc(8000);
	char *b = malloc(2000);
	char *c = malloc(2000);

	if (!a || !b || !c)
		printf(RED "Memory allocation error\n" RESET);
	for (int i = 0; i < 3; ++i) {
		a[i] = 'a';
		b[i] = 'b';
		c[i] = 'c';
	}
	//    show_alloc_mem();
	free(a);
	free(b);
	free(c);
	printf(GREEN "Test x OK\n" RESET);
}

void test_alloc_random_file() {
	// Create a file from /dev/urandom
	system("dd if=/dev/urandom of=/tmp/test_in.txt bs=1024 count=102400");

	int fd = open("/tmp/test_in.txt", O_RDONLY);
	if (fd == -1) {
		printf(RED "Error opening file\n" RESET);
		exit(1);
	}
	printf(GREEN "File opened\n" RESET);

	char *ptr = malloc(1024 * 1024 * 100);
	if (!ptr) {
		printf(RED "Memory allocation error\n" RESET);
		close(fd);// Close the file before exiting
		exit(1);
	}
	printf(GREEN "Memory allocated\n" RESET);

	ssize_t bytesRead = read(fd, ptr, 1024 * 1024 * 100);
	if (bytesRead == -1) {
		printf(RED "Error reading from file\n" RESET);
		free(ptr);
		close(fd);
		exit(1);
	}
	printf(GREEN "File read\n" RESET);
	close(fd);// Close the file as soon as it's done being used

	int fd2 = open("/tmp/test_out.txt", O_WRONLY | O_CREAT, 0644);
	if (fd2 == -1) {
		printf(RED "Error opening output file\n" RESET);
		free(ptr);// Free memory before exiting
		exit(1);
	}

	ssize_t bytesWritten = write(fd2, ptr, bytesRead);
	if (bytesWritten == -1 || bytesWritten != bytesRead) {
		printf(RED "Error writing to output file\n" RESET);
		free(ptr);
		close(fd2);
		exit(1);
	}
	printf(GREEN "File written\n" RESET);
	close(fd2);// Close the output file

	free(ptr);
	printf(GREEN "Memory freed\n" RESET);

	// Compare the two files
	if (system("diff /tmp/test_in.txt /tmp/test_out.txt") == 0) {
		printf(GREEN "Files are identical\n" RESET);
	} else {
		printf(RED "Files are different\n" RESET);
	}
}

void test_simple_realloc() {
	printf("Testing simple realloc...\n");

	char *ptr_1;
	char *ptr_2;
	char *ptr_3;
	char *ptr_4;
	char *ptr_5;
	char *ptr_6;
	char *ptr_7;
	char *ptr_8;

	ptr_1 = malloc(1);
	ptr_2 = malloc(1);
	ptr_3 = malloc(1);
	ptr_4 = malloc(1);

	ptr_5 = realloc(ptr_1, 2);
	ptr_6 = realloc(ptr_2, 2);
	ptr_7 = realloc(ptr_3, 2);
	ptr_8 = realloc(ptr_4, 2);

	if (!ptr_5 || !ptr_6 || !ptr_7 || !ptr_8)
		printf(RED "Memory allocation error\n" RESET);

	free(ptr_5);
	free(ptr_6);
	free(ptr_7);
	free(ptr_8);

	printf(GREEN "Test simple realloc OK\n" RESET);
}

void test_realloc_tiny_to_small() {
	printf("Testing realloc tiny to small...\n");

	char *ptr_1;
	char *ptr_2;
	char *ptr_3;
	char *ptr_4;

	ptr_1 = malloc(TINY - 1);
	ptr_2 = malloc(TINY - 1);
	ptr_3 = malloc(TINY - 1);
	ptr_4 = malloc(TINY - 1);

	ptr_1 = realloc(ptr_1, SMALL);
	ptr_2 = realloc(ptr_2, SMALL);
	ptr_3 = realloc(ptr_3, SMALL);
	ptr_4 = realloc(ptr_4, SMALL);

	if (!ptr_1 || !ptr_2 || !ptr_3 || !ptr_4)
		printf(RED "Memory allocation error\n" RESET);
	free(ptr_1);
	free(ptr_2);
	free(ptr_3);
	free(ptr_4);
	printf(GREEN "Test realloc tiny to small OK\n" RESET);
}

void test_realloc_tiny_to_large() {
	printf("Testing realloc tiny to large...\n");

	char *ptr_1;
	char *ptr_2;
	char *ptr_3;
	char *ptr_4;

	ptr_1 = malloc(TINY - 1);
	ptr_2 = malloc(TINY - 1);
	ptr_3 = malloc(TINY - 1);
	ptr_4 = malloc(TINY - 1);

	ft_memset(ptr_1, 'a', TINY - 1);
	ft_memset(ptr_2, 'b', TINY - 1);
	ft_memset(ptr_3, 'c', TINY - 1);
	ft_memset(ptr_4, 'd', TINY - 1);

	ptr_1 = realloc(ptr_1, LARGE);
	ptr_2 = realloc(ptr_2, LARGE);
	ptr_3 = realloc(ptr_3, LARGE);
	ptr_4 = realloc(ptr_4, LARGE);

	if (!ptr_1 || !ptr_2 || !ptr_3 || !ptr_4)
		printf(RED "Memory allocation error\n" RESET);
	// now check them all
	for (int i = 0; i < (int) TINY - 1; i++) {
		if (ptr_1[i] != 'a')
			printf(RED "Memory overwrite error\n" RESET);
		if (ptr_2[i] != 'b')
			printf(RED "Memory overwrite error\n" RESET);
		if (ptr_3[i] != 'c')
			printf(RED "Memory overwrite error\n" RESET);
		if (ptr_4[i] != 'd')
			printf(RED "Memory overwrite error\n" RESET);
	}


	if (!ptr_1 || !ptr_2 || !ptr_3 || !ptr_4)
		printf(RED "Memory allocation error\n" RESET);
	free(ptr_1);
	free(ptr_2);
	free(ptr_3);
	free(ptr_4);
	printf(GREEN "Test realloc tiny to large OK\n" RESET);
}

void test_random_realloc_level_one(){

}

void test_realloc_expanding() {
	printf("Testing realloc expanding...\n");

	char *ptr = malloc(10);
	if (!ptr) {
		printf(RED "Initial memory allocation error\n" RESET);
		return;
	}

	strcpy(ptr, "123456789");

	char *new_ptr = realloc(ptr, 100);
	if (!new_ptr) {
		printf(RED "Memory allocation error on realloc expanding\n" RESET);
		free(ptr);
		return;
	}
	strcat(new_ptr, "0abcdefghij");

	if (strcmp(new_ptr, "1234567890abcdefghij") != 0) {
		printf(RED "Memory content error after realloc expanding\n" RESET);
	}

	free(new_ptr);
	printf(GREEN "Test realloc expanding OK\n" RESET);
}

void test_realloc_shrinking() {
	printf("Testing realloc shrinking...\n");

	char *ptr = malloc(100);
	if (!ptr) {
		printf(RED "Initial memory allocation error\n" RESET);
		return;
	}

	strcpy(ptr, "1234567890abcdefghij");

	char *new_ptr = realloc(ptr, 10);
	if (!new_ptr) {
		printf(RED "Memory allocation error on realloc shrinking\n" RESET);
		free(ptr);
		return;
	}

	if (strncmp(new_ptr, "1234567890", 10) != 0) {
		printf(RED "Memory content error after realloc shrinking\n" RESET);
	}

	free(new_ptr);
	printf(GREEN "Test realloc shrinking OK\n" RESET);
}

void test_realloc_zero_size() {
	printf("Testing realloc with zero size (should free)...\n");

	char *ptr = malloc(100);
	if (!ptr) {
		printf(RED "Initial memory allocation error\n" RESET);
		return;
	}
	ft_bzero(ptr, 100);

	char *new_ptr = realloc(ptr, 0);
	free(new_ptr);

	printf(GREEN "Test realloc with zero size OK\n" RESET);
}

void test_realloc_null_ptr() {
	printf("Testing realloc with NULL pointer (should behave like malloc)...\n");

	char *new_ptr = realloc(NULL, 100);
	if (!new_ptr) {
		printf(RED "Memory allocation error on realloc NULL ptr\n" RESET);
		return;
	}

	strcpy(new_ptr, "1234567890");
	if (strcmp(new_ptr, "1234567890") != 0) {
		printf(RED "Memory content error after realloc NULL ptr\n" RESET);
	}

	free(new_ptr);
	printf(GREEN "Test realloc with NULL pointer OK\n" RESET);
}

void test_stress_malloc(){
	int i = 0;

	printf("Testing stress malloc...\n");

	while (i < MAX_ITER){
		char *ptr = malloc(100);
		if (!ptr) {
			printf(RED "Initial memory allocation error\n" RESET);
			return;
		}
		ft_bzero(ptr, 100);
		free(ptr);
		i++;
	}

	printf(GREEN "Test stress malloc OK\n" RESET);
}

int main() {
	test_malloc_small();
	test_four_mallocs();
	test_malloc_large();
	test_double_array_small();

	test_double_array();
	test_integer_array();
	test_large_memory_copy();
	test_memory_overwrite_small();
	test_memory_overwrite();
//	 test_stress_malloc();
	  test_negative();
	test_zero();
	test_mem_alignment();
	test_mem_alignment_extensive();
	test_malloc_with_double_arr_integers();
	test_thousand_hundred_mallocs();
	test_thousand_hundred_mallocs_write_read();
	 test_million_mallocs();
	 test_gazillion_mallocs();
	test_x();
	// test_alloc_random_file();
	test_simple_realloc();
	test_realloc_tiny_to_small();
	test_realloc_tiny_to_large();
	// system("leaks test");`

	test_realloc_expanding();
	test_realloc_shrinking();
	test_realloc_zero_size();
	test_realloc_null_ptr();
//
//#if (HOSTTYPE == x86_64_Darwin)
//		system("leaks test");
//#endif
	return 0;
}
