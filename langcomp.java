class Element implements Comparable<Element> {
	int data;
	int max;
	
	public Element (int data_, int max_) { data = data_; max = max_; }
	public int compareTo (Element other) {
		if (data >= max || other.data >= max)
			throw new RuntimeException ("Invalid number.");
		return data - other.data;
	}
	public String toString () {
		return Integer.toString (data);
	}
}

class Langcomp {
	public static void main (String [] args) {
		if (args.length == 4) {
			System.out.printf ("Java : %9dµs\n", run (Integer.parseInt(args[0]), Integer.parseInt(args[1]), Integer.parseInt(args[2]), args [3].equals ("y")));
		} else {
			System.out.print ("Usage: java Langcomp MAX SIZE SEED PRINT\n" +
					"  MAX = Max number\n" +
					"  SIZE = Numbers to sort\n" +
					"  SEED = srand() seed\n" +
					"  PRINT = y/n - whether to print result\n");
		}
	}
	
	static public long run (int max, int size, int seed, boolean print) {
		long begin = System.nanoTime ();
		
		Element [] input = new Element [size];
		Element [] output = new Element [size];
		
		java.util.Random r = new java.util.Random (seed);
		
		for (int i = 0; i < size; i++) {
			input [i] = new Element (r.nextInt (max), max);
		}
		
		mergesort (input, output);
		if (print) System.out.println (java.util.Arrays.toString (output));
		
		long end = System.nanoTime ();
		return (end-begin) / 1000;
	}

	static <T extends Comparable<T>> void mergesort (T [] input, T [] output) {
		mergesort (input, output, 0, input.length, true);
	}


	static <T extends Comparable<T>> void merge (T [] input, T [] output, int pos, int size1, int size2) {
		int p = 0, q = 0;
		for (int i = 0; i < size1+size2; ++i) {
			if (q == size2 || (p < size1 &&    input[pos+p].compareTo (input[pos+size1+q]) < 0)) {
				output [pos+i] = input [pos+p];
				++p;
			} else {
				output [pos+i] = input [pos+size1+q];
				++q;
			}
		}
	}

	static <T extends Comparable<T>> void mergesort (T [] input, T [] output, int pos, int size, boolean mode) {
		if (size == 1) {
			if (mode) {
				output [pos] = input [pos];
			}
		} else {
			int size1 = size/2;
			int size2 = size-size1;
			mergesort (input, output, pos		,	size1,	!mode);
			mergesort (input, output, pos+size1	,	size2,	!mode);
			
			if (mode)
				merge (input, output, pos, size1, size2);
			else
				merge (output, input, pos, size1, size2);
		}
	}

}
