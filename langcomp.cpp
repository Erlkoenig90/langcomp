#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <iomanip>

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

uint64_t run (unsigned int max, size_t size, unsigned int seed, bool print) {
	using T = unsigned int;
	using Clock = std::chrono::steady_clock;

	Clock::time_point begin = Clock::now ();	
	
	std::vector<T> vec (size); // { 4, 3, 2, 1 };
	std::srand (seed);
	std::generate (vec.begin (), vec.end (), [=] () { return std::rand () % max; });

	try {
		mergesort (vec, [=] (T a, T b) {
			if (a >= max || b >= max) throw std::logic_error ("Invalid number");
			return a < b;
		});
		if (print) output (vec);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what ();
	}
	
	Clock::time_point end = Clock::now ();
	return std::chrono::duration_cast<std::chrono::microseconds> (end-begin).count ();
}

int main (int argc, char* argv []) {
	std::vector<std::string> args (argv, argv+argc);

	if (args.size () == 5) {
		std::cout << "C++  : " << std::setw(9) << run (std::stoi (args [1]), std::stoi (args [2]), std::stoi (args [3]), args [4] == "y") << "µs" << std::endl;
	} else {
		printf ("Usage: %s MAX SIZE SEED PRINT\n"
				"  MAX = Max number\n"
				"  SIZE = Numbers to sort\n"
				"  SEED = srand() seed\n"
				"  PRINT = y/n - whether to print result\n", argv [0]);
	}
}
