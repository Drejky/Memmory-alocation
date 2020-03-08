#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* start;

void *memory_alloc(unsigned int size) {
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);
	return 0;
}

int memory_free(void *valid_ptr) {
	return 0;
}

int memory_check(void *ptr) {
	return 0;
}

void memory_init(void *ptr, unsigned int size) {
	start = (char*)ptr; 
	/*
	*((int*)start) = (size - 2*sizeof(int)) * -1;
	*((int*)(start + size - sizeof(int))) = (size - 2 * sizeof(int)) * -1;
	*/
	*((unsigned short*)start) = size - 2 * sizeof(unsigned short) - sizeof(char);
	*((char*)start + sizeof(unsigned short)) = 0;
	*((unsigned short*)(start + size - sizeof(unsigned short))) = size - 2 * sizeof(unsigned short) + sizeof(char);
	printf("%d", *(start));

}

int main()
{
	char region[10];
	memory_init(region, 10);/*
	char* pointer = (char*)memory_alloc(10);
	if (pointer)
		memset(pointer, 0, 10);
	if (pointer)
		memory_free(pointer);*/
	return 0;
}

//unsigned byte uwu;
