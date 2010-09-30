CCFILES := $(shell find -name '*.cc')
CXXFLAGS=-W -Wall -I../../..
CC=g++

all:

depend:
	$(CXX) $(CXXFLAGS) -MM $(CCFILES) > makefile.deps

clean:
	rm *.o

-include makefile.deps