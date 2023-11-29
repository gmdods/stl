#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "algorithm.hpp"
#include "numeric.hpp"

int main() {

	auto same = [](auto begin, auto end, auto data) {
		return std::equal(begin, end, data.cbegin(), data.cend());
	};

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

	auto [match_a, match_c] =
	    loop::mismatch(a.cbegin(), a.cend(), c.cbegin());
	assert(std::next(a.cbegin(), 6) == match_a);
	assert(std::next(c.cbegin(), 6) == match_c);

	assert(a.begin() == loop::find_if(a.cbegin(), a.cend(), odd));
	auto three_a = loop::find(a.cbegin(), a.cend(), 3);
	assert(3 == *three_a);
	assert(std::next(b.cbegin(), 4) ==
	       loop::adjacent_find(b.cbegin(), b.cend()));

	std::vector<int> buffer{};
	auto out = std::back_inserter(buffer);

	loop::copy(a.cbegin(), a.cend(), out);
	assert(same(a.cbegin(), a.cend(), buffer));
	buffer.clear();

	loop::copy_if(a.cbegin(), a.cend(), out, lt_3);
	assert(same(a.cbegin(), three_a, buffer));
	buffer.clear();

	loop::copy_n(a.cbegin(), std::distance(a.cbegin(), three_a), out);
	assert(same(a.cbegin(), three_a, buffer));
	buffer.clear();

	loop::remove_copy_if(a.cbegin(), a.cend(), out, lt_3);
	assert(same(three_a, a.cend(), buffer));
	buffer.clear();

	loop::transform(a.cbegin(), std::next(a.cend(), -2), out,
			[](auto elt) { return elt + 1; });
	assert(same(std::next(a.cbegin()), std::next(a.cend(), -1), buffer));
	buffer.clear();

	loop::replace_copy_if(a.cbegin(), a.cend(), out, lt_3, 2);
	assert(buffer[0] == 2);
	assert(buffer[1] == 2);
	buffer.erase(buffer.begin(), std::next(buffer.begin(), 2));
	assert(same(std::next(a.cbegin(), 2), a.cend(), buffer));
	buffer.clear();

	assert(a.cbegin() == loop::min_element(a.cbegin(), a.cend()));
	assert(6 == *loop::max_element(a.cbegin(), a.cend()));

	assert(loop::equal(a.cbegin(), a.cend(), a.cbegin()));

	assert(loop::is_partitioned(a.cbegin(), a.cend(), lt_3));
	assert(loop::find_if_not(a.cbegin(), a.cend(), lt_3) ==
	       loop::partition_point(a.cbegin(), a.cend(), lt_3));

	auto last = std::next(a.cend(), -1);
	assert(loop::is_sorted(a.cbegin(), last));
	assert(last == loop::is_sorted_until(a.cbegin(), a.cend()));

	auto three_b = loop::find(b.cbegin(), b.cend(), 3);
	auto rng = loop::range{loop::lower_bound(b.cbegin(), b.cend(), 3),
			       loop::upper_bound(b.cbegin(), b.cend(), 3)};
	assert(loop::binary_search(b.cbegin(), b.cend(), 3));
	assert(three_b == std::next(b.cbegin(), 4));
	assert(three_b == rng.f);
	assert(std::next(three_b, 3) == rng.l);
	assert(rng == loop::equal_range(b.cbegin(), b.cend(), 3));

	assert(25 == loop::reduce(a.cbegin(), a.cend(), 0, std::plus{}));
	assert(0 == loop::transform_reduce(a.cbegin(), a.cend(), 25,
					   std::plus{}, std::negate{}));
	assert(-3 == loop::inner_product(a.cbegin(), a.cend(), c.cbegin(), 0,
					 std::plus{}, std::minus{}));

	std::cout << "OK!" << '\n';
	return 0;
}
