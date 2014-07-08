#define _POSIX_C_SOURCE (199309L)
#define __STDC_FORMAT_MACROS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>


typedef int (*compare_t) (void*, void*, void*);

int merge (void* input, void* output, size_t pos, size_t size1, size_t size2, size_t els, compare_t f, void* userdata) {
	size_t p = 0, q = 0;
	for (size_t i = 0; i < size1+size2; ++i) {
		int ok = (q == size2) ? 1 : 0;
		if (!ok && p < size1) {
			int res = f (input + ((pos+p)*els), input + ((pos+size1+q)*els), userdata);
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

int mergesortInt (void* input, void* output, size_t pos, size_t size, int mode, size_t els, compare_t f, void* userdata) {
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
		if (!mergesortInt (input, output, pos		,	size1,	1-mode, els, f, userdata)) return 0;
		if (!mergesortInt (input, output, pos+size1	,	size2,	1-mode, els, f, userdata)) return 0;
		
		if (mode) {
			if (!merge (input, output, pos, size1, size2, els, f, userdata)) return 0;
		} else {
			if (!merge (output, input, pos, size1, size2, els, f, userdata)) return 0;
		}
	}
	return 1;
}

int mergesort (void** input, size_t size, size_t els, compare_t f, void* userdata) {
	void* temp = malloc (size * els);
	
	int r = mergesortInt (*input, temp, 0, size, 1, els, f, userdata);
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

int myCompare (void* ap, void* bp, void* userdata) {
	unsigned int max = *((unsigned int*) userdata);
	unsigned int a = *((unsigned int*) ap);
	unsigned int b = *((unsigned int*) bp);
	if (a >= max || b >= max)
		return -1;
	return (a < b) ? 1 : 0;
}

uint64_t getUS (struct timespec* t) {
	return (((uint64_t) t->tv_sec) * 1000000) + (t->tv_nsec / 1000);
}

uint64_t run (unsigned int max, size_t size, unsigned int seed, int print) {

	struct timespec begin, end;
	clock_gettime (CLOCK_MONOTONIC, &begin);
	
	unsigned int* vec = (unsigned int*) malloc (size * sizeof (unsigned int));

	srand (seed);
	for (size_t i = 0; i < size; i++) {
		vec [i] = rand () % max;
	}
	
	if (!mergesort ((void*) &vec, size, sizeof (unsigned int), &myCompare, (void*) &max)) {
		fprintf (stderr, "Error.\n");
	} else {
		if (print)
			output (vec, size);
	}
	
	free (vec);

	clock_gettime (CLOCK_MONOTONIC, &end);
	return getUS (&end) - getUS (&begin);
}

int main (int argc, char* argv []) {
	if (argc == 5) {
		printf ("C    : %9" PRIu64 "µs\n", run (atoi (argv [1]), atoi (argv [2]), atoi (argv [3]), argv [4][0] == 'y'));
	} else {
		printf ("Usage: %s MAX SIZE SEED PRINT\n"
				"  MAX = Max number\n"
				"  SIZE = Numbers to sort\n"
				"  SEED = srand() seed\n"
				"  PRINT = y/n - whether to print result\n", argv [0]);
	}
}

