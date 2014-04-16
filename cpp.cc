#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

#include "conf.h"

template <typename T, typename F>
void merge (std::vector<T>& input, std::vector<T>& output, size_t pos, size_t size1, size_t size2, F f) {
	size_t p = 0, q = 0;
	for (size_t i = 0; i < size1+size2; ++i) {
		if (q == size2 || (p < size1 && f (input [pos+p], input [pos+size1+q]))) {
			output [pos+i] = input [pos+p];
			++p;
		} else {
			output [pos+i] = input [pos+size1+q];
			++q;
		}
	}
}

template <typename T, typename F>
void mergesort (std::vector<T>& input, std::vector<T>& output, size_t pos, size_t size, bool mode, F f) {
//	std::cout << "mergesort(" << (&input) << ", " << (&output) << ", " << pos << ", " << size << ", " << mode << ");\n";
	/*
		mode = true
			input => output
	*/
	if (size == 1) {
		if (mode) {
			output [pos] = input [pos];
		}
	} else {
		size_t size1 = size/2;
		size_t size2 = size-size1;
		mergesort (input, output, pos		,	size1,	!mode, f);
		mergesort (input, output, pos+size1	,	size2,	!mode, f);
		
		if (mode)
			merge (input, output, pos, size1, size2, f);
		else
			merge (output, input, pos, size1, size2, f);
	}
}

template <typename T, typename F>
void mergesort (std::vector<T>& input, F f) {
	std::vector<T> temp (input.size ());
	
	mergesort (input, temp, 0, input.size (), true, f);
	input = std::move (temp);
}

template <typename T>
void output (std::vector<T>& vec) {
	std::cout << '{';
	for (auto v : vec) {
		std::cout << v << ", ";
	}
	std::cout << "}\n";
}

int main () {
	using T = unsigned int;
	const size_t size = SORT_SIZE;
	std::vector<T> vec (size); // { 4, 3, 2, 1 };
	std::srand (SEED);
	std::generate (vec.begin (), vec.end (), [] () { return std::rand () % DAT_MAX; });

//	output (vec);
	
	try {
		mergesort (vec, [] (T a, T b) {
			if (a >= DAT_MAX || b >= DAT_MAX)
				throw std::logic_error ("Invalid number");
			return a < b;
		});
		if (PRINT_RES)
			output (vec);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what ();
	}
	
}

