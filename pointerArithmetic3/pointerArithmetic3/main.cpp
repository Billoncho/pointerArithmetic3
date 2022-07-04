/*
Author:     Billy Gene Ridgeway
Purpose:    Learn pointer math and why the size of an array changes with the same items in different 
            order and how C aligns data types in memory.
Date:       July 4th. 2022 Happy Independence Day!
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int a;           // 4 bytes    
	double b;        // 8 bytes    
	int c;           // 4 bytes    
	long long int d; // 8 bytes
} MYSTRUCT;

#define COUNT 4

// A more complicated example.

int main() {
	MYSTRUCT *p, *q;
	void* v;

	printf("size of MYSTRUCT = %d\n", sizeof(MYSTRUCT));
	// IMPORTANT NOTE: The size of MYSTRUCT is NOT always 24 bytes! It will vary according to the processor architecture
	// ...
	// ... because some processor architectures will require a 'double' (say) to be at an address which is multiple of 8
	// ...
	// ... so it is very important to make no assumptions on the size of a structure: ALWAYS use 'sizeof' or let the
	// compiler use address arithmetic.

	// Allocate some memory and clean it out: 'calloc' is quite useful here.
	//
	
	// *********** p = (MYSTRUCT*)calloc(COUNT, 24); // DO NOT DO THIS! ***********
	 p = (MYSTRUCT*)calloc(COUNT, sizeof(MYSTRUCT));
	// Initialize it to something interesting using a large floating point value for the double and a large value for
	// the 64-bit integer.
	// This is so we can see where the compiler has inserted padding space to get the desired alignment for the 8 byte
	// quantities 'b' and 'd'.
	for (int i = 0; i < COUNT; i++) {
		p[i].a = i;
		p[i].b = 10000000000.0 + i;
		p[i].c = i * 20;
		p[i].d = 4294967296 + i;
	}

	// First let's see what we've got.
	// NOTE: Look at the addresses of the 'double' and the 'long long int' - they start (are aligned) on an 8 byte
	// boundary.
	q = p;
	printf("[0] values:\na is %d\nb is %f\nc is %d\nd is %lld\n", q->a, q->b, q->c, q->d);
	printf("addresses:\na is %p\nb is %p\nc is %p\nd is %p\n", &q->a, &q->b, &q->c, &q->d);
	q = p + 3; // Use address arithmetic to get the address of the fourth element (the array is zero based remember).
	printf("\n[3] values:\na is %d\nb is %f\nc is %d\nd is %lld\n", q->a, q->b, q->c, q->d);
	printf("addresses:\na is %p\nb is %p\nc is %p\nd is %p\n\n", &q->a, &q->b, &q->c, &q->d);

	// Finally a more advanced way of looking at what is inside the structure (all 32 bytes of it).
	// IMPORTANT NOTE: Address arithmetic will not work with **generic pointers** unless you use a cast
	// this will not work: 'v = v + 3' (or 'v[3]') because the compiler doesn't know what the thing that 'v' (a generic
	// pointer) is pointing to is ...
	// ... and so it can't use address arithmetic; instead we have to use a cast to tell the compiler what 'v' is
	// pointing at.
	
	v = p + 3; // Look at the fourth element of the array of MYSTRUCTs.
			   // Print out the values as integers - the zeros show you where the padding has been inserted by the compiler.
			   // NOTE: That we are using address arithmetic to move along MYSTRUCT as a set of 8 'int' values of 4 bytes each (32
			   // bytes in total).
	
	printf("Struct at index 3\n");
	for (int i = 0; i < sizeof(MYSTRUCT) / sizeof(int); i++) {
		printf("v[%d]=%d\n", i, ((int*)v)[i]);		
	}
    
    // v[0]=3 because it holds an integer, 4 bytes.
    // v[1]=0 because it holds padding or /0 from the calloc function.
    // v[2]=538443776 because it holds the first 4 bytes of a double.
    // v[3]=1107468383 because it holds the second 4 bytes of a double.
    // v[4]=60 because it holds an integer, 4 bytes.
    // v[5]=0 because it holds padding or /0 from the calloc function.
    // v[6]=3 because it holds the first 4 bytes of a long long int.
    // v[7]=1 because it holds the last 4 bytes of a long long int.

	return 0;
}
