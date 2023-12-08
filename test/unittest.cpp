#ifndef UNITTEST_MAIN
#include "unittest.h"

#include <array>
#include <assert.h>
#include <functional>
#include <iostream>
#include <vector>

#include "../include/algorithm.hpp"
#include "../include/numeric.hpp"

#endif

constexpr auto same = [](auto begin, auto end, auto data) {
	return std::equal(begin, end, data.cbegin(), data.cend());
};

constexpr auto odd = [](auto i) { return static_cast<bool>(i % 2); };
constexpr auto lt_3 = [](auto i) { return i < 3; };

#define LET_A() \
	std::array a { 1, 2, 3, 4, 5, 6, 4 }
#define LET_B() \
	std::array b { -1, 0, 1, 2, 3, 3, 3, 4 }
#define LET_C() \
	std::array c { 1, 2, 3, 4, 5, 6, 7 }
#define LET_D() \
	std::array d { 0, 1, 3, 6, 10, 15, 21, 28 }
#define LET_OUTPUT(o, out) \
	std::vector<int> o{}; \
	o.reserve(16); \
	auto out = std::back_inserter(o)

unittest("all_of, any_of, none_of") {
	LET_A();

	ensure(loop::all_of(a.cbegin(), a.cend(), loop::fn::lt(7)));
	ensure(loop::any_of(a.cbegin(), a.cend(), loop::fn::eq(3)));
	ensure(loop::none_of(a.cbegin(), a.cend(), loop::fn::gt(6)));
}

unittest("for_each, for_each_n") {
	LET_A();

	size_t sum = 0;
	loop::for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	ensure(25 == sum);
	sum = 0;
	ensure(std::next(a.cbegin(), 2) ==
	       loop::for_each_n(a.cbegin(), 2, [&sum](auto i) { sum += i; }));
	ensure(3 == sum);
}

unittest("count, count_if") {
	LET_A();

	ensure(3 == loop::count_if(a.cbegin(), a.cend(), odd));
	ensure(1 == loop::count(a.cbegin(), a.cend(), 3));
}

unittest("mismatch") {
	LET_A();
	LET_C();

	auto [match_a, match_c] =
	    loop::mismatch(a.cbegin(), a.cend(), c.cbegin());
	ensure(std::next(a.cbegin(), 6) == match_a);
	ensure(std::next(c.cbegin(), 6) == match_c);
}

unittest("find, find_if") {
	LET_A();

	ensure(a.begin() == loop::find_if(a.cbegin(), a.cend(), odd));
	ensure(3 == *loop::find(a.cbegin(), a.cend(), 3));
}

unittest("adjacent_find") {
	LET_B();

	ensure(std::next(b.cbegin(), 4) ==
	       loop::adjacent_find(b.cbegin(), b.cend()));
}

unittest("search, find_first_of") {
	LET_A();
	LET_B();

	auto three_a = loop::find(a.cbegin(), a.cend(), 3);

	ensure(three_a == loop::find_first_of(a.cbegin(), a.cend(), three_a,
					      std::next(three_a)));
	ensure(three_a == loop::search(a.cbegin(), a.cend(),
				       std::prev(b.cend(), 2),
				       std::prev(b.cend())));
}

unittest("copy, copy_if, copy_n, transform") {
	LET_A();
	LET_OUTPUT(o, out);

	auto three_a = loop::find(a.cbegin(), a.cend(), 3);

	loop::copy(a.cbegin(), a.cend(), out);
	ensure(same(a.cbegin(), a.cend(), o));
	o.clear();

	loop::copy_if(a.cbegin(), a.cend(), out, lt_3);
	ensure(same(a.cbegin(), three_a, o));
	o.clear();

	loop::copy_n(a.cbegin(), std::distance(a.cbegin(), three_a), out);
	ensure(same(a.cbegin(), three_a, o));
	o.clear();

	loop::transform(a.cbegin(), std::next(a.cend(), -2), out,
			[](auto elt) { return elt + 1; });
	ensure(same(std::next(a.cbegin()), std::prev(a.cend()), o));
	o.clear();
}

unittest("rotate_copy, unique_copy") {
	LET_A();
	LET_B();
	LET_OUTPUT(o, out);

	auto it_a = std::next(loop::find(a.cbegin(), a.cend(), 3));
	loop::copy(a.cbegin(), it_a, out);
	loop::rotate_copy(it_a, std::prev(a.cend()), a.cend(), out);
	ensure(loop::is_sorted(o.cbegin(), o.cend()));
	o.clear();

	auto it_b = std::next(loop::find(b.cbegin(), b.cend(), 3));
	loop::unique_copy(b.cbegin(), std::prev(b.cend()), out);
	ensure(same(b.cbegin(), it_b, o));
	o.clear();
}

unittest("remove_copy(_if), replace_copy(_if)") {
	LET_A();
	LET_OUTPUT(o, out);

	auto three_a = loop::find(a.cbegin(), a.cend(), 3);

	loop::remove_copy_if(a.cbegin(), a.cend(), out, lt_3);
	ensure(same(three_a, a.cend(), o));
	o.clear();

	loop::replace_copy_if(a.cbegin(), a.cend(), out, lt_3, 2);
	ensure(o[0] == 2);
	ensure(o[1] == 2);
	o.erase(o.begin(), std::next(o.begin(), 2));
	ensure(same(std::next(a.cbegin(), 2), a.cend(), o));
	o.clear();
}

unittest("generate, fill, generate_n, fill_n") {
	LET_B();
	LET_C();
	LET_OUTPUT(o, out);

	auto three_b = loop::find(b.cbegin(), b.cend(), 3);

	o.assign(c.size(), 0);
	int i = 0;
	loop::generate(o.begin(), o.end(), [&i]() { return ++i; });
	ensure(same(c.cbegin(), c.cend(), o));
	o.clear();

	o.assign(3, 0);
	loop::fill(o.begin(), o.end(), 3);
	ensure(same(three_b, std::prev(b.cend()), o));
	o.clear();

	o.assign(c.size(), 0);
	int j = 0;
	loop::generate_n(o.begin(), c.size(), [&j]() { return ++j; });
	ensure(same(c.cbegin(), c.cend(), o));
	o.clear();

	o.assign(3, 0);
	loop::fill_n(o.begin(), 3, 3);
	ensure(same(three_b, std::prev(b.cend()), o));
	o.clear();
}

unittest("min_element, max_element, minmax_element") {
	LET_A();

	ensure(a.cbegin() == loop::min_element(a.cbegin(), a.cend()));
	ensure(6 == *loop::max_element(a.cbegin(), a.cend()));
}

unittest("equal") {
	LET_A();

	ensure(loop::equal(a.cbegin(), a.cend(), a.cbegin()));
}

unittest("is_partitioned, partition_point") {
	LET_A();

	ensure(loop::is_partitioned(a.cbegin(), a.cend(), lt_3));
	ensure(loop::find_if_not(a.cbegin(), a.cend(), lt_3) ==
	       loop::partition_point(a.cbegin(), a.cend(), lt_3));
}
unittest("partition_copy") {
	LET_A();
	LET_OUTPUT(o, out);
	LET_OUTPUT(o_, out_);

	auto three_a = loop::find(a.cbegin(), a.cend(), 3);

	loop::partition_copy(a.cbegin(), a.cend(), out, out_, lt_3);
	ensure(same(a.cbegin(), three_a, o));
	ensure(same(three_a, a.cend(), o_));
	o.clear();
}

unittest("is_sorted, is_sorted_until") {
	LET_A();

	auto last = std::prev(a.cend());
	ensure(loop::is_sorted(a.cbegin(), last));
	ensure(last == loop::is_sorted_until(a.cbegin(), a.cend()));
}

unittest("binary_search, lower_bound, upper_bound, equal_range") {
	LET_B();

	auto three_b = loop::find(b.cbegin(), b.cend(), 3);

	auto rng = loop::range{loop::lower_bound(b.cbegin(), b.cend(), 3),
			       loop::upper_bound(b.cbegin(), b.cend(), 3)};
	ensure(loop::binary_search(b.cbegin(), b.cend(), 3));
	ensure(three_b == std::next(b.cbegin(), 4));
	ensure(three_b == rng.f);
	ensure(std::next(three_b, 3) == rng.l);
	ensure(rng == loop::equal_range(b.cbegin(), b.cend(), 3));
}

unittest("iota") {
	LET_C();
	LET_OUTPUT(o, out);

	o.assign(c.size(), 0);
	loop::iota(o.begin(), o.end(), 1);
	ensure(same(c.cbegin(), c.cend(), o));
	o.clear();
}

unittest("reduce, transform_reduce, inner_product, adjancent_difference") {
	LET_A();
	LET_C();
	LET_OUTPUT(o, out);

	ensure(25 == loop::reduce(a.cbegin(), a.cend(), 0, std::plus{}));
	ensure(0 == loop::transform_reduce(a.cbegin(), a.cend(), 25,
					   std::plus{}, std::negate{}));
	ensure(-3 == loop::inner_product(a.cbegin(), a.cend(), c.cbegin(), 0,
					 std::plus{}, std::minus{}));

	loop::adjancent_difference(c.cbegin(), c.cend(), out, std::minus{});
	ensure(all_of(o.cbegin(), o.cend(), loop::fn::eq(1)));
	o.clear();
}

unittest("inclusive_scan, exclusive_scan, partial_sum") {
	LET_C();
	LET_D();
	LET_OUTPUT(o, out);

	loop::inclusive_scan(c.cbegin(), c.cend(), out, 0, std::plus{});
	ensure(same(std::next(d.cbegin()), d.cend(), o));
	o.clear();
	loop::exclusive_scan(c.cbegin(), c.cend(), out, 0, std::plus{});
	ensure(same(d.cbegin(), std::prev(d.cend()), o));
	o.clear();
	loop::partial_sum(c.cbegin(), c.cend(), out, std::plus{});
	ensure(same(std::next(d.cbegin()), d.cend(), o));
	o.clear();
}

unittest("transform_inclusive_scan, transform_exclusive_scan") {
	LET_C();
	LET_D();
	LET_OUTPUT(o, out);

	loop::transform_inclusive_scan(c.cbegin(), c.cend(), out, 0,
				       std::minus{}, std::negate{});
	ensure(same(std::next(d.cbegin()), d.cend(), o));
	o.clear();
	loop::transform_exclusive_scan(c.cbegin(), c.cend(), out, 0,
				       std::minus{}, std::negate{});
	ensure(same(d.cbegin(), std::prev(d.cend()), o));
	o.clear();
}
