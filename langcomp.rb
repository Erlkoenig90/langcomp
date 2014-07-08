#!/usr/bin/env ruby
# coding: utf-8

def merge (input, output, pos, size1, size2, f)
	p = 0
	q = 0
	for i in 0...(size1+size2)
		if(q == size2 || (p < size1 && f.call(input[pos+p], input[pos+size1+q])))
			output[pos+i] = input[pos+p];
			p += 1;
		else
			output[pos+i] = input[pos+size1+q];
			q += 1;
		end
	end
end

def mergesortI(input, output, pos, size, mode, f)
	if(size == 1)
		if(mode)
			output [pos] = input [pos];
		end
	else
		size1 = size/2;
		size2 = size-size1;
		mergesortI(input, output, pos			,	size1,	!mode, f);
		mergesortI(input, output, pos+size1,	size2,	!mode, f);
		
		if (mode)
			merge(input, output, pos, size1, size2, f)
		else
			merge(output, input, pos, size1, size2, f)
		end
	end
end

def mergesort (input, &f)
	temp = Array.new(input.size());
	
	mergesortI(input, temp, 0, input.size(), true, f);
	temp
end

def run (max, size, seed, print)
	tBegin = Time.now	
	
	srand(seed)
	vec = Array.new(size) { |i| rand(max) } 

	begin
		mergesort(vec) { |a, b|
			if (a >= max || b >= max)
				raise "Invalid number"
			end
			a < b
		}
		if (print)
			p vec
		end
	rescue => ex
		puts "#{ex.backtrace}: #{ex.message} (#{ex.class})"
	end
	
	tEnd = Time.now
	(tEnd-tBegin) * 1000000
end

if(ARGV.size == 4)
	printf("ruby : %9dµs\n", run(ARGV[0].to_i, ARGV[1].to_i, ARGV[2].to_i, ARGV[3] == "y"))
else
	puts("Usage: ruby langcomp.rb MAX SIZE SEED PRINT\n" +
				"  MAX = Max number\n" +
				"  SIZE = Numbers to sort\n" +
				"  SEED = srand() seed\n" +
				"  PRINT = y/n - whether to print result\n");
end
