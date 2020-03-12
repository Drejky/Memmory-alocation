//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* start;

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
			if (oldSize - hSize - fSize - size >= hSize + fSize){			//If we have space left for another free block we give it a header and footer
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
	
	//Marking given block as free
	*((char*)(curr - sizeof(char))) = 0;

	//Defragmenting with previous blocks
	//LOOPS ARE USELESS FIX LATER
	while ((curr - hSize) >= start + sizeof(unsigned short)) {		//While our curr pointer isnt out of bounds	
		if ((curr - hSize) == start + sizeof(unsigned short)) {		//If we are on the first block
			break;
		}
		else {		//If we aren't on the first block we calculate size of current and previous block
			prevSize = *((unsigned short*)(curr - hSize - fSize));
			currSize = *((unsigned short*)(curr - hSize));
		}


		if (*((char*)(curr - prevSize - sizeof(char))) == 0) {		//If previous block is free
			newSize = currSize + prevSize;		//Calculating size of merged block
			*((unsigned short*)(curr - hSize - prevSize)) = newSize;	//New blocks header
			*((unsigned short*)(curr - hSize + currSize - fSize)) = newSize;	//New blocks footer
			curr = curr - prevSize;		//Putting our cursor to the start of our new block
			continue;
		}
		else {
			break;
		}
	}
	//Defragmenting with following blocks
	while ((curr - hSize + *((unsigned short*)(curr - hSize))) <= (start + *((unsigned short*)start))) {	//While our curr pointer isnt out of bounds
		if ((curr - hSize + *((unsigned short*)(curr - hSize))) == (start + *((unsigned short*)start))) {	//If we are on the last block
			break;
		}
		else {		//If we aren't on the last block we calculate size of current and next block
			currSize = *((unsigned short*)(curr - hSize));
			nextSize = *((unsigned short*)(curr + currSize - hSize));
		}

		if (*((char*)(curr + currSize - sizeof(char))) == 0) {
			newSize = currSize + nextSize;
			*((unsigned short*)(curr - hSize)) = newSize;
			*((unsigned short*)(curr - hSize + currSize + nextSize - fSize)) = newSize;
			continue;
		}
		else {
			break;
		}
	}

	return 0;
}

int memory_check(void *ptr) {
	if (ptr == NULL) 
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
	char region[102];
	memory_init(region, 102);
	char* pointer = (char*)memory_alloc(1);
	char* pointer2 = (char*)memory_alloc(1);
	char* pointer3 = (char*)memory_alloc(1);
	char* pointer4 = (char*)memory_alloc(1);
	char* pointer5 = (char*)memory_alloc(1);
	char* pointer6 = (char*)memory_alloc(1);
	char* pointer7 = (char*)memory_alloc(1);

	pointer[0] = 'u';
	
	memory_free(pointer7);
	memory_free(pointer6);
	memory_free(pointer5);
	memory_free(pointer4);

	return 0;
}

//unsigned byte uwu;
