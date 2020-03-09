//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* start;

void *memory_alloc(unsigned int size) {
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);
	unsigned short oldSize;

	char* curr = start;
	
	while (1) {
		if (*((char*)curr + sizeof(unsigned short)) == 0 && *(unsigned short*)curr > (hSize + fSize + size)) {
			oldSize = *((unsigned short*)curr);
			*(unsigned short*)curr = hSize + size + fSize;
			*((char*)(curr + sizeof(unsigned short))) = 1;	//Rewriting header
			*((unsigned short*)(curr + size + hSize)) = hSize + size + fSize;  //Creating new footer for allocated block
			if (oldSize - hSize - fSize - size >= 5){			//If we have space for another free block left we give it a header and footer
				*((unsigned short*)(curr + size + hSize + fSize)) = oldSize - hSize - fSize - size;
				*((char*)(curr + size + hSize + fSize + sizeof(unsigned short))) = 0;
				*((unsigned short*)(curr + oldSize - fSize)) = oldSize - hSize - fSize - size;	//Rewriting the original footer
			}
			return (curr + hSize);
			
		}
		else if (*((char*)curr + sizeof(unsigned short)) == 0 && *(unsigned short*)curr == (hSize + fSize + size)) {	//If the free block fits perfectly we just mark it as allocated and return pointer
			*((char*)(curr + sizeof(unsigned short))) = 1;
			return (curr + hSize);
		}
		else {
			curr += *((unsigned short*)curr);
			continue;
		}
		return NULL;
	}

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
	*((unsigned short*)start) = size;
	*((char*)start + sizeof(unsigned short)) = 0;
	*((unsigned short*)(start + size - sizeof(unsigned short))) = size;

}

int main()
{
	char region[30];
	memory_init(region, 30);
	char* pointer = (char*)memory_alloc(10);
	char* pointer2 = (char*)memory_alloc(9);
	pointer[0] = 'u';
	/*
	if (pointer)
		memset(pointer, 0, 10);
	if (pointer)
		memory_free(pointer);*/
	return 0;
}

//unsigned byte uwu;
