#include <array>
#include <cassert>
#include <functional>
#include <iostream>

#include "algorithm.hpp"
#include "numeric.hpp"

int main() {
	using namespace loop;

	auto odd = [](auto i) { return static_cast<bool>(i % 2); };

	std::array a{1, 2, 3, 4, 5, 6};

	assert(all_of(a.cbegin(), a.cend(), [](auto i) { return i <= 6; }));
	assert(any_of(a.cbegin(), a.cend(), [](auto i) { return i == 3; }));
	assert(none_of(a.cbegin(), a.cend(), [](auto i) { return i > 6; }));

	size_t sum = 0;
	for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	assert(21 == sum);

	assert(3 == count_if(a.cbegin(), a.cend(), odd));
	assert(1 == count(a.cbegin(), a.cend(), 3));

	assert(a.begin() == find_if(a.cbegin(), a.cend(), odd));
	assert(3 == *find(a.cbegin(), a.cend(), 3));

	assert(a.begin() == min_element(a.cbegin(), a.cend()));
	assert(6 == *max_element(a.cbegin(), a.cend()));

	assert(21 == reduce(a.cbegin(), a.cend(), 0, std::plus{}));
	assert(0 == transform_reduce(a.cbegin(), a.cend(), 21, std::plus{},
				     std::negate{}));

	std::cout << "OK!" << '\n';
	return 0;
}
