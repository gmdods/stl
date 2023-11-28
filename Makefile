CXX = g++
CXXFLAGS = -Wall -Wpedantic -Wextra -Wno-unused-variable \
	-Wno-unused-function -march=native -O3 -std=c++2a
DEPS = loop.hpp fn.hpp
FILES = algorithm.hpp numeric.hpp

test: test.out
.PHONY: test

test.o: test.cpp $(DEPS) $(FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.out: test.o
	$(CXX) $(CXXFLAGS) $? -o $@ && ./$@

clean:
	rm *.o *.out
