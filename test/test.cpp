#include <array>
#include <assert.h>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "../include/algorithm.hpp"
#include "../include/numeric.hpp"

#define UNITTEST_MAIN
#include "unittest.h"

int main(int argc, const char * argv[]) {

#include "unittest.cpp"

	summary();
	return 0;
}
