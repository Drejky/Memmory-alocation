//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* start;

void *memory_alloc(unsigned int size) {	
	int hSize = sizeof(unsigned short) + sizeof(char);		//Header size
	int fSize = sizeof(unsigned short);					//Footer size
	unsigned short oldSize;

	char* curr = start + sizeof(unsigned short);	//Putting our pointer to the start of first block
	
	while (curr < (start + *((unsigned short*)start))) {
		if (*((char*)curr + sizeof(unsigned short)) == 0 && *(unsigned short*)curr >= (hSize + fSize + size)) {	//If block is marked as free (char at thrid byte of header) and its size is big enough to hold our block
			oldSize = *((unsigned short*)curr);

			if (oldSize - hSize - fSize - size >= hSize + fSize) {			//If we have space left for another free block we give it a header and footer
				*((unsigned short*)(curr + size + hSize + fSize)) = oldSize - hSize - fSize - size;		//New header for next free block
				*((char*)(curr + size + hSize + fSize + sizeof(unsigned short))) = 0;
				*((unsigned short*)(curr + oldSize - fSize)) = oldSize - hSize - fSize - size;	//Rewriting the original footer
			}
			else {
				size += oldSize - hSize - fSize - size;		//If there isn't enough space for a new block we give the block padding
			}

			*(unsigned short*)curr = hSize + size + fSize;
			*((char*)(curr + sizeof(unsigned short))) = 1;	//Rewriting header
			*((unsigned short*)(curr + hSize + size)) = hSize + size + fSize;  //Creating new footer for allocated block

			return (void*)(curr + hSize);
			
		}
		else {			//Moving to the next block
			curr += *((unsigned short*)curr);
			continue;
		}
	}

	return NULL;	//Return null if we didnt find a fitting free block
}

int memory_free(void *valid_ptr) {
	int prevSize, nextSize, currSize, newSize;
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);
	char* curr = (char*)valid_ptr;

	if (!curr)
		return 1;
	
	//Marking given block as free
	*((char*)(curr - sizeof(char))) = 0;

	//Merging with following block
	if (!((curr - hSize + *((unsigned short*)(curr - hSize))) == (start + *((unsigned short*)start)))) {	//If we aren't on the last block
		currSize = *((unsigned short*)(curr - hSize));
		nextSize = *((unsigned short*)(curr + currSize - hSize));

		if (*((char*)(curr + currSize - sizeof(char))) == 0) {
			newSize = currSize + nextSize;
			*((unsigned short*)(curr - hSize)) = newSize;
			*((unsigned short*)(curr - hSize + currSize + nextSize - fSize)) = newSize;
		}
	}

	//Merging with previous block	
	if (!((curr - hSize) == start + sizeof(unsigned short))) {		//If we aren't on the first block
		prevSize = *((unsigned short*)(curr - hSize - fSize));
		currSize = *((unsigned short*)(curr - hSize));

		if (*((char*)(curr - prevSize - sizeof(char))) == 0) {		//If previous block is free
			newSize = currSize + prevSize;		//Calculating size of merged block
			*((unsigned short*)(curr - hSize - prevSize)) = newSize;	//New blocks header
			*((unsigned short*)(curr - hSize + currSize - fSize)) = newSize;	//New blocks footer
		}
	}

	return 0;
}

int memory_check(void *ptr) {
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);

	char* curr = start + sizeof(unsigned short);

	if (ptr && ptr >= start && ptr <= (start + *((unsigned short*)start))) {	//If given pointer is valid and within our given block
		while (curr < (start + *((unsigned short*)start))) {
			if (*((char*)((char*)ptr - sizeof(char))) == 1 && ptr == (curr + hSize)) {	//checking wether block is allocated and if it matches current blocks returned pointer
				return 1;
			}
			else
				curr += *((unsigned short*)curr);
		}
	}

	return 0;
}

void memory_init(void *ptr, unsigned int size) {
	start = (char*)ptr;			//Setting our global pointer to given region
	*((unsigned short*)start) = size;		//Setting first 2 bytes of our memmory to its size
	*((unsigned short*)(start + sizeof(unsigned short))) = size - sizeof(unsigned short);		//Creating header for our first block
	*((char*)start + 2*sizeof(unsigned short)) = 0;			//Marking our first block as free
	*((unsigned short*)(start + size - sizeof(unsigned short))) = size - sizeof(unsigned short);		//Creating footer for our first block
}

int rando(int low, int high) {	//random numbers for testing
	return rand() % (high - low + 1) + low;
}

void test_sameSmall() {
	printf("Test sameSmall\n");
	char region[50];
	memory_init(region, 50);

	char* pointers[100];

	for (int i = 0; i < 50; i++) {
		pointers[i] = (char*)memory_alloc(8);
	}

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));

	for (int i = 0; i < 50; i++)
		memory_free(pointers[i]);
	printf("\n");

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));
	printf("\n-----------------------------------------------\n");
}

void test_randSmall() {
	printf("Test randSmall\n");
	char region[150];
	memory_init(region, 150);
	srand(time(0));

	char* pointers[100];

	for (int i = 0; i < 50; i++) {
		pointers[i] = (char*)memory_alloc(rando(8,24));
	}

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));

	for (int i = 0; i < 50; i++)
		memory_free(pointers[i]);
	printf("\n");

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));
	printf("\n-----------------------------------------------\n");
}

void test_randBig() {
	printf("Test randBig\n");
	char region[10000];
	memory_init(region, 10000);
	srand(time(0));

	char* pointers[100];

	for (int i = 0; i < 50; i++) {
		pointers[i] = (char*)memory_alloc(rando(500, 5000));
	}

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));

	for (int i = 0; i < 50; i++)
		memory_free(pointers[i]);
	printf("\n");

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));
	printf("\n-----------------------------------------------\n");

}

void test_randMix() {
	printf("Test randMix\n");
	char region[50000];
	memory_init(region, 50000);
	srand(time(0));

	char* pointers[100];

	for (int i = 0; i < 50; i++) {
		pointers[i] = (char*)memory_alloc(rando(8, 50000));
	}

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));

	for (int i = 0; i < 50; i++)
		memory_free(pointers[i]);
	printf("\n");

	for (int i = 0; i < 50; i++)
		printf("%d ", memory_check(pointers[i]));
	printf("\n-----------------------------------------------\n");

}

void test_freeFirst() {
	char region[32];
	memory_init(region, 32);

	char* pointers[100];

	for (int i = 0; i < 3; i++)
		pointers[i] = (char*)memory_alloc(5);
	memory_free(pointers[0]);
}

void test_freeLast() {
	char region[32];
	memory_init(region, 32);

	char* pointers[100];

	for (int i = 0; i < 3; i++)
		pointers[i] = (char*)memory_alloc(5);
	memory_free(pointers[2]);
}

void test_freeMergeLeft() {
	char region[32];
	memory_init(region, 32);

	char* pointers[100];

	for (int i = 0; i < 3; i++)
		pointers[i] = (char*)memory_alloc(5);

	for(int i = 2; i >= 0; i--)
		memory_free(pointers[i]);
}

void test_freeMergeRight() {
	char region[32];
	memory_init(region, 32);

	char* pointers[100];

	for (int i = 0; i < 3; i++)
		pointers[i] = (char*)memory_alloc(5);

	for (int i = 0; i < 3; i++)
		memory_free(pointers[i]);
}

void test_freeMergeBoth() {
	char region[32];
	memory_init(region, 32);

	char* pointers[100];

	for (int i = 0; i < 3; i++)
		pointers[i] = (char*)memory_alloc(5);

	memory_free(pointers[0]);
	memory_free(pointers[2]);
	memory_free(pointers[1]);
}

int main()
{
	//test_sameSmall();
	//test_randSmall();
	//test_randBig();
	//test_randMix();

	//test_freeFirst();
	//test_freeLast();
	//test_freeMergeLeft();
	//test_freeMergeRight();
	//test_freeMergeBoth();

	return 0;
}

