all : langcomp_c langcomp_cpp Langcomp.class

CFLAGS=-Wall -Wextra -Werror -O3 -march=native -mtune=native

langcomp_c : langcomp.c
	gcc $(CFLAGS) $^ -o $@ -std=c99

langcomp_cpp : langcomp.cpp
	g++ $(CFLAGS) -std=c++11 $^ -o $@

Langcomp.class : langcomp.java
	javac $<

clean :
	rm -f langcomp_c langcomp_cpp Langcomp.class

MAX = 1000
SIZE = 1000000
SEED = 42
PRINT = n

run : all
	@echo "Language comparison: Sorting ${SIZE} numbers"
	@./langcomp_cpp ${MAX} ${SIZE} ${SEED} ${PRINT}
	@./langcomp_c ${MAX} ${SIZE} ${SEED} ${PRINT}
	@java Langcomp ${MAX} ${SIZE} ${SEED} ${PRINT}
	@ruby langcomp.rb ${MAX} ${SIZE} ${SEED} ${PRINT}