CXX = g++
CXXFLAGS = -Wall -Wpedantic -Wextra \
	   -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter \
	   -march=native -Og -g -std=c++2a
DEPS = ../include/loop.hpp ../include/fn.hpp
FILES = ../include/algorithm.hpp ../include/numeric.hpp

test: test.out
.PHONY: test

test.o: test.cpp $(DEPS) $(FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.out: test.o
	$(CXX) $(CXXFLAGS) $? -o $@ && ./$@

clean:
	rm *.o *.out
