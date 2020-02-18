CXX = g++
CCFLAGS = -Wall -Wextra -pedantic -Werror -std=c++17 -g

# this is a "Suffix Rule" - how to create a .o from a .cc file
.cpp.o:
	$(CXX) $(CCFLAGS) -c -O3 $<

all: MemoryBenchmark

MemoryBenchmark: MemoryBenchmark.o
	$(CXX) $(CCFLAGS) -o benchmark MemoryBenchmark.o

MemoryBenchmark.o:	MemoryBenchmark.cc
	$(CXX) $(CCFLAGS) -c MemoryBenchmark.cc


clean:
	rm -f *.o
