#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "algorithm.hpp"
#include "numeric.hpp"

int main() {

	auto odd = [](auto i) { return static_cast<bool>(i % 2); };

	std::array a{1, 2, 3, 4, 5, 6};

	assert(
	    loop::all_of(a.cbegin(), a.cend(), [](auto i) { return i <= 6; }));
	assert(
	    loop::any_of(a.cbegin(), a.cend(), [](auto i) { return i == 3; }));
	assert(
	    loop::none_of(a.cbegin(), a.cend(), [](auto i) { return i > 6; }));

	size_t sum = 0;
	loop::for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	assert(21 == sum);

	assert(3 == loop::count_if(a.cbegin(), a.cend(), odd));
	assert(1 == loop::count(a.cbegin(), a.cend(), 3));

	assert(a.begin() == loop::find_if(a.cbegin(), a.cend(), odd));
	assert(3 == *loop::find(a.cbegin(), a.cend(), 3));

	assert(a.begin() == loop::min_element(a.cbegin(), a.cend()));
	assert(6 == *loop::max_element(a.cbegin(), a.cend()));

	assert(21 == loop::reduce(a.cbegin(), a.cend(), 0, std::plus{}));
	assert(0 == loop::transform_reduce(a.cbegin(), a.cend(), 21,
					   std::plus{}, std::negate{}));

	std::cout << "OK!" << '\n';
	return 0;
}
