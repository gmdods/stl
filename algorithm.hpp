#ifndef LOOP_STL_ALGORITHM_HPP
#define LOOP_STL_ALGORITHM_HPP

#include <stdlib.h>

#include "fn.hpp"
#include "loop.hpp"

namespace loop {

// Non-modifying

template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	loop::element_each(f, l, fn);
}

template <typename It, typename Fn>
constexpr It for_each_n(It f, size_t n, Fn fn) {
	auto guard = [&n, fn](auto elt) {
		if (n-- == 0)
			return false;
		else {
			std::invoke(fn, elt);
			return true;
		}
	};
	return loop::element_while(f, nullptr, guard).it;
}

template <typename It, typename Fn>
constexpr size_t count_if(It f, It l, Fn fn) {
	size_t count = 0;
	loop::for_each(f, l,
		       [&count, fn](auto elt) { count += fn::bit(fn, elt); });
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return loop::count_if(f, l, fn::eq(val));
}

template <typename It, typename Fn>
constexpr bool all_of(It f, It l, Fn fn) {
	return loop::element_while(f, l, fn).ended();
}

template <typename It, typename Fn>
constexpr bool none_of(It f, It l, Fn fn) {
	return loop::all_of(f, l, fn::ifnot(fn));
}

template <typename It, typename Fn>
constexpr bool any_of(It f, It l, Fn fn) {
	return !loop::none_of(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if_not(It f, It l, Fn fn) {
	return loop::element_while(f, l, fn).it;
}

template <typename It, typename Fn>
constexpr It find_if(It f, It l, Fn fn) {
	return loop::find_if_not(f, l, fn::ifnot(fn));
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return loop::find_if(f, l, fn::eq(val));
}

template <typename It>
constexpr It adjacent_find(It f, It l) {
	return loop::adjacent_while(f, l, std::not_equal_to{}).it;
}

// Min/Max

template <typename It>
struct minmax {
	It min, max;
};

template <typename It>
constexpr minmax<It> minmax_element(It f, It l) {
	It max = f;
	It min = f;
	loop::iterator_each(f, l, [&max, &min](auto it) {
		if (*it > *max) max = it;
		if (*it < *min) min = it;
	});
	return {min, max};
}

template <typename It>
constexpr It max_element(It f, It l) {
	return loop::minmax_element(f, l).max;
}

template <typename It>
constexpr It min_element(It f, It l) {
	return loop::minmax_element(f, l).min;
}

// Partition

template <typename It, typename Fn>
constexpr bool is_partitioned(It f, It l, Fn fn) {
	f = loop::find_if_not(f, l, fn);
	return loop::none_of(f, l, fn);
}

template <typename It, typename Fn>
constexpr It partition_point(It f, It l, Fn fn) {
	return loop::binary_bound(f, l, fn);
}

// Sorts

template <typename It>
constexpr It is_sorted_until(It f, It l) {
	auto it = loop::adjacent_while(f, l, std::less{}).it;
	++it;
	return it;
}

template <typename It>
constexpr bool is_sorted(It f, It l) {
	return loop::adjacent_while(f, l, std::less{}).ended();
}

// Binary Search

template <typename It, typename T>
constexpr bool binary_search(It f, It l, T val) {
	return loop::binary(f, l, fn::lt(val), fn::ifnot(fn::eq(val))).found();
}

template <typename It, typename T>
constexpr It lower_bound(It f, It l, T val) {
	return loop::partition_point(f, l, fn::lt(val));
}

template <typename It, typename T>
constexpr It upper_bound(It f, It l, T val) {
	return loop::partition_point(f, l, fn::ifnot(fn::gt(val)));
}

template <typename It, typename T>
constexpr range<It> equal_range(It f, It l, T val) {
	auto ret = loop::binary(f, l, fn::lt(val), fn::ifnot(fn::eq(val)));
	auto [lb, ub] = ret.it;
	if (ret.found()) {
		auto mid = loop::midpoint(lb, ub);
		return {loop::lower_bound(lb, mid, val),
			loop::upper_bound(mid, ub, val)};
	} else {
		return {lb, ub};
	}
}

} // namespace loop

#endif // !LOOP_STL_ALGORITHM_HPP
