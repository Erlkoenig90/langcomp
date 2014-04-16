all : c cpp

c : c.c conf.h
	gcc -Wall -Wextra -Werror $^ -o $@ -std=c99 -O3

cpp : cpp.cc conf.h
	g++ -std=c++11 $^ -o $@ -Wall -Wextra -Werror -O3

clean :
	rm c cpp

run : c cpp
	time ./c
	time ./cpp
