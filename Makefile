CXX = g++
CCFLAGS = -Wall -Wextra -pedantic --std=c++17 -O3 

# this is a "Suffix Rule" - how to create a .o from a .cc file
.cpp.o:
	$(CXX) $(CCFLAGS) -c $<

all: MemoryBenchmark

MemoryBenchmark: MemoryBenchmark.o
	$(CXX) $(CCFLAGS) -o benchmark MemoryBenchmark.o

MemoryBenchmark.o:	MemoryBenchmark.cc
	$(CXX) $(CCFLAGS) -c MemoryBenchmark.cc


clean:
	rm -f *.o
