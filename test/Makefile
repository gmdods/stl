CXX = g++
CXXFLAGS = -Wall -Wpedantic -Wextra \
	   -Wno-unused-variable -Wno-unused-but-set-variable \
	   -Wno-unused-function -Wno-unused-parameter \
	   -march=native -Og -g -std=c++2a
DEPS = ../include/loop.hpp ../include/fn.hpp
FILES = ../include/algorithm.hpp ../include/numeric.hpp

test: test.out
.PHONY: test

test.out: test.cpp $(DEPS) $(FILES)
	$(CXX) $(CXXFLAGS) $< -o $@ && ./$@

verbose: verbose.out
.PHONY: verbose

verbose.out: test.cpp $(DEPS) $(FILES)
	$(CXX) $(CXXFLAGS) -DUNITTEST_VERBOSE $< -o $@ && ./$@

clean:
	rm *.out
