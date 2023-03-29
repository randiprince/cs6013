#Makefile - Operating Systems Malloc Replacement
#Targets and what they do:
#MyMalloc creates the executable
#*.o targets compile .cpp and .h files into an o file
#clean removes all files and executables

CXXSOURCE = main.o HashTable.o MyMalloc.o
HEADERS = HashTable.hpp MyMalloc.hpp
CXXFLAGS = --std=c++14
COMPILER = c++
COMPILEOPTS = -c
LINKINGOPTS = -o

run: MyMalloc
	./MyMalloc

MyMalloc: $(CXXSOURCE)
	$(COMPILER) $(CXXFLAGS) $(LINKINGOPTS) MyMalloc $(CXXSOURCE)

.PHONY: clean

main.o: main.cpp $(HEADERS)
	$(COMPILER) $(CXXFLAGS) $(COMPILEOPTS) main.cpp

HashTable.o: HashTable.cpp $(HEADERS)
	$(COMPILER) $(CXXFLAGS) $(COMPILEOPTS) HashTable.cpp

MyMalloc.o: HashTable.cpp $(HEADERS)
	$(COMPILER) $(CXXFLAGS) $(COMPILEOPTS) MyMalloc.cpp

clean:
	rm -rf *.o MyMalloc
