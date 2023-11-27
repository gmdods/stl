#include <array>
#include <cassert>

#include "algorithm.cpp"

int main() {
	std::array a{1, 2, 3, 4, 5, 6};

	std::size_t sum = 0;
	for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	assert(sum == 21);
	return 0;
}
