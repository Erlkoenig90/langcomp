#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "conf.h"

typedef int (*compare_t) (void*, void*);

int merge (void* input, void* output, size_t pos, size_t size1, size_t size2, size_t els, compare_t f) {
	size_t p = 0, q = 0;
	for (size_t i = 0; i < size1+size2; ++i) {
		int ok = (q == size2) ? 1 : 0;
		if (!ok && p < size1) {
			int res = f (input + ((pos+p)*els), input + ((pos+size1+q)*els) );
			if (res == -1) return 0;
			if (res == 1) ok = 1;
		}
		if (ok) {
			memcpy (output+((pos+i)*els), input + ((pos+p)*els), els);
			++p;
		} else {
			memcpy (output+((pos+i)*els), input + ((pos+size1+q)*els), els);
			++q;
		}
	}
	return 1;
}

int mergesortInt (void* input, void* output, size_t pos, size_t size, int mode, size_t els, compare_t f) {
//	std::cout << "mergesort(" << (&input) << ", " << (&output) << ", " << pos << ", " << size << ", " << mode << ");\n";
	/*
		mode = true
			input => output
	*/
	if (size == 1) {
		if (mode) {
			memcpy (output + (pos * els), input + (pos * els), els);
		}
	} else {
		size_t size1 = size/2;
		size_t size2 = size-size1;
		if (!mergesortInt (input, output, pos		,	size1,	1-mode, els, f)) return 0;
		if (!mergesortInt (input, output, pos+size1	,	size2,	1-mode, els, f)) return 0;
		
		if (mode) {
			if (!merge (input, output, pos, size1, size2, els, f)) return 0;
		} else {
			if (!merge (output, input, pos, size1, size2, els, f)) return 0;
		}
	}
	return 1;
}

int mergesort (void** input, size_t size, size_t els, compare_t f) {
	void* temp = malloc (size * els);
	
	int r = mergesortInt (*input, temp, 0, size, 1, els, f);
	free (*input);
	*input = temp;
	return r;
}

// -----------

void output (void* vec, size_t size) {
	printf ("{");
	for (size_t i = 0; i < size; i++) {
		printf ("%u, ", ((unsigned int*) vec)[i]);
	}
	printf ("}\n");
}

int myCompare (void* ap, void* bp) {
	unsigned int a = *((unsigned int*) ap);
	unsigned int b = *((unsigned int*) bp);
	if (a >= DAT_MAX || b >= DAT_MAX)
		return -1;
	return (a < b) ? 1 : 0;
}

int main () {
	const size_t size = SORT_SIZE;
	unsigned int* vec = (unsigned int*) malloc (size * sizeof (unsigned int));

	srand (SEED);
	for (size_t i = 0; i < size; i++) {
		vec [i] = rand () % DAT_MAX;
	}
	
	if (!mergesort ((void*) &vec, size, sizeof (unsigned int), &myCompare)) {
		fprintf (stderr, "Error.\n");
	} else {
		if (PRINT_RES)
			output (vec, size);
	}
	
	free (vec);
}

