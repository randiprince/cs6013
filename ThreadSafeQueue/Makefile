#Makefile - Operating Systems Unix Shell
#Targets and what they do:
#UnixShell creates the executable
#*.o targets compile .cpp and .h files into an o file
#clean removes all files and executables

CXXSOURCE = ConcurrentQueueTest.o
HEADERS = ConcurrentQueue.hpp
CXXFLAGS = --std=c++14
COMPILER = c++
COMPILEOPTS = -c
LINKINGOPTS = -o

ConcurrentQueue: $(CXXSOURCE)
	$(COMPILER) $(CXXFLAGS) $(LINKINGOPTS) ConcurrentQueue $(CXXSOURCE)

.PHONY: clean

run: ConcurrentQueue
	./ConcurrentQueue

ConcurrentQueueTest.o: ConcurrentQueueTest.cpp $(HEADERS)
	$(COMPILER) $(CXXFLAGS) $(COMPILEOPTS) ConcurrentQueueTest.cpp

clean:
	rm -rf *.o ConcurrentQueue
