#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "algorithm.hpp"
#include "numeric.hpp"

int main() {

	auto odd = [](auto i) { return static_cast<bool>(i % 2); };
	auto lt_3 = loop::fn::lt(3);

	std::array a{1, 2, 3, 4, 5, 6, 4};
	std::array b{-1, 0, 1, 2, 3, 3, 3, 4};
	std::array c{1, 2, 3, 4, 5, 6, 7};

	assert(loop::all_of(a.cbegin(), a.cend(), loop::fn::lt(7)));
	assert(loop::any_of(a.cbegin(), a.cend(), loop::fn::eq(3)));
	assert(loop::none_of(a.cbegin(), a.cend(), loop::fn::gt(6)));

	size_t sum = 0;
	loop::for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	assert(25 == sum);
	sum = 0;
	assert(std::next(a.cbegin(), 2) ==
	       loop::for_each_n(a.cbegin(), 2, [&sum](auto i) { sum += i; }));
	assert(3 == sum);

	assert(3 == loop::count_if(a.cbegin(), a.cend(), odd));
	assert(1 == loop::count(a.cbegin(), a.cend(), 3));

	auto place =
	    std::pair{std::next(a.cbegin(), 6), std::next(c.cbegin(), 6)};
	assert(place == loop::mismatch(a.cbegin(), a.cend(), c.cbegin()));

	assert(a.begin() == loop::find_if(a.cbegin(), a.cend(), odd));
	assert(3 == *loop::find(a.cbegin(), a.cend(), 3));
	assert(std::next(b.cbegin(), 4) ==
	       loop::adjacent_find(b.cbegin(), b.cend()));

	assert(a.cbegin() == loop::min_element(a.cbegin(), a.cend()));
	assert(6 == *loop::max_element(a.cbegin(), a.cend()));

	assert(loop::equal(a.cbegin(), a.cend(), a.cbegin()));

	assert(loop::is_partitioned(a.cbegin(), a.cend(), lt_3));
	assert(loop::find_if_not(a.cbegin(), a.cend(), lt_3) ==
	       loop::partition_point(a.cbegin(), a.cend(), lt_3));

	auto last = std::next(a.cend(), -1);
	assert(loop::is_sorted(a.cbegin(), last));
	assert(last == loop::is_sorted_until(a.cbegin(), a.cend()));

	auto three = loop::find(b.cbegin(), b.cend(), 3);
	auto rng = loop::range{loop::lower_bound(b.cbegin(), b.cend(), 3),
			       loop::upper_bound(b.cbegin(), b.cend(), 3)};
	assert(loop::binary_search(b.cbegin(), b.cend(), 3));
	assert(three == std::next(b.cbegin(), 4));
	assert(three == rng.f);
	assert(std::next(three, 3) == rng.l);
	assert(rng == loop::equal_range(b.cbegin(), b.cend(), 3));

	assert(25 == loop::reduce(a.cbegin(), a.cend(), 0, std::plus{}));
	assert(0 == loop::transform_reduce(a.cbegin(), a.cend(), 25,
					   std::plus{}, std::negate{}));
	assert(-3 == loop::inner_product(a.cbegin(), a.cend(), c.cbegin(), 0,
					 std::plus{}, std::minus{}));

	std::cout << "OK!" << '\n';
	return 0;
}
