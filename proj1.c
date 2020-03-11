//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* start;

int a = 5;


void *memory_alloc(unsigned int size) {
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);
	unsigned short oldSize;

	char* curr = start + sizeof(unsigned short);	//Putting our pointer to the start of first block
	
	while (curr < (start + *((unsigned short*)start))) {
		if (*((char*)curr + sizeof(unsigned short)) == 0 && *(unsigned short*)curr > (hSize + fSize + size)) {
			oldSize = *((unsigned short*)curr);
			*(unsigned short*)curr = hSize + size + fSize;
			*((char*)(curr + sizeof(unsigned short))) = 1;	//Rewriting header
			*((unsigned short*)(curr + size + hSize)) = hSize + size + fSize;  //Creating new footer for allocated block
			if (oldSize - hSize - fSize - size >= 5){			//If we have space left for another free block we give it a header and footer
				*((unsigned short*)(curr + size + hSize + fSize)) = oldSize - hSize - fSize - size;		//New header for next free block
				*((char*)(curr + size + hSize + fSize + sizeof(unsigned short))) = 0;
				*((unsigned short*)(curr + oldSize - fSize)) = oldSize - hSize - fSize - size;	//Rewriting the original footer
			}
			return (void*)(curr + hSize);
			
		}
		else if (*((char*)curr + sizeof(unsigned short)) == 0 && *(unsigned short*)curr == (hSize + fSize + size)) {	//If the free block fits perfectly we just mark it as allocated and return pointer
			*((char*)(curr + sizeof(unsigned short))) = 1;
			return (void*)(curr + hSize);
		}
		else {			//Moving to the next block
			curr += *((unsigned short*)curr);
			continue;
		}
	}

	return NULL;
}

int memory_free(void *valid_ptr) {
	int prevSize, nextSize, currSize, newSize;
	int hSize = sizeof(unsigned short) + sizeof(char);
	int fSize = sizeof(unsigned short);
	char* curr = (char*)valid_ptr;
	
	prevSize = *((unsigned short*)(curr - hSize - fSize));
	currSize = *((unsigned short*)(curr - hSize));


	//Marking given block as free
	*((char*)(curr - sizeof(char))) = 0;

	//Defragmenting 
	//FIX WHERE YOU CALCULATE SIZES
	while ((curr - prevSize - hSize) > start) {		//While our curr pointer isnt out of bounds
		//Getting the block sizes
		prevSize = *((unsigned short*)(curr - hSize - fSize));
		currSize = *((unsigned short*)(curr - hSize));

		//If we find that previous block is free
		if ((curr - prevSize - hSize) > start && *((char*)(curr - prevSize - sizeof(char))) == 0) {	
			newSize = currSize + prevSize;		//Calculating size of merged block
			*((unsigned short*)(curr - prevSize - hSize)) = newSize;	//Asigning size to new blocks header
			*((unsigned short*)(curr + currSize - hSize - fSize)) = newSize;
			curr = curr - prevSize;
			continue;
		}
		else
			break;
	}
	while ((curr + currSize) < (start + *((unsigned short*)start))) {	//While our curr pointer isnt out of bounds
		currSize = *((unsigned short*)(curr - hSize));
		nextSize = *((unsigned short*)(curr + currSize - hSize));

		if ((curr + currSize) < (start + *((unsigned short*)start)) && *((char*)(curr + currSize - sizeof(char))) == 0) {
			newSize = currSize + nextSize;
			*((unsigned short*)(curr - hSize)) = newSize;
			*((unsigned short*)(curr - hSize + currSize + nextSize - fSize)) = newSize;
			continue;
		}
		else
			break;
	}

	return 0;
}

int memory_check(void *ptr) {
	return 0;
}

void memory_init(void *ptr, unsigned int size) {
	start = (char*)ptr;			//Setting our global pointer to given region
	*((unsigned short*)start) = size;		//Setting first 2 bytes of our memmory to its size
	*((unsigned short*)(start + sizeof(unsigned short))) = size - sizeof(unsigned short);		//Creating header for our first block
	*((char*)start + 2*sizeof(unsigned short)) = 0;			//Marking our first block as free
	*((unsigned short*)(start + size - sizeof(unsigned short))) = size - sizeof(unsigned short);		//Creating footer for our first block

}

int main()
{
	char region[52];
	memory_init(region, 52);
	char* pointer = (char*)memory_alloc(5);
	char* pointer2 = (char*)memory_alloc(5);
	char* pointer3 = (char*)memory_alloc(5);
	char* pointer4 = (char*)memory_alloc(5);

	pointer[0] = 'u';
	
	memory_free(pointer);
	memory_free(pointer4);

	return 0;
}

//unsigned byte uwu;
