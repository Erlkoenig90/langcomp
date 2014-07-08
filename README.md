langcomp
========
This is a tiny benchmark for the following programming languages regarding type-generic algorithms.
Specifically, a simple version of mergesort is implemented in a way that it accepts arrays of any type and uses a user-supplied comparison
function. This is done according to the languages' principles.

### Currently included languages:
C++, C, java, ruby

### Usage
g++, gcc, java, javac, and ruby have to be installed. Compile the programs via "make". Run the benchmarks via "make run".
Different parameters (size of array to be sorted) can be configured by editing the makefile.

### The generic algorithms are implemented via...
* C++ : templates
* C   : void* passing, function pointer passing, and casting
* Java: The Comparable interface, i.e. virtual functions
* ruby: Using ruby's dynamic type system.

### Results on my computer:
|Language|Time|
|--------|---:|
|C++|200914µs|
|C|426275µs|
|Java|738789µs|
|ruby|12875743µs|

### License:
BSD. See LICENSE file.
