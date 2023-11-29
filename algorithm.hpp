#ifndef LOOP_STL_ALGORITHM_HPP
#define LOOP_STL_ALGORITHM_HPP

#include <stdlib.h>
#include <utility>

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
	auto g = fn::guard([&n](auto) { return (n-- == 0); }, fn);
	return loop::element_while(f, nullptr, g).it;
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
constexpr std::pair<It, It> mismatch(It f, It l, It s) {
	auto g = fn::guard([&s](auto elt) { return (elt != *s); },
			   [&s](auto) { ++s; });
	auto it = find_if_not(f, l, g);
	return {it, s};
}

template <typename It>
constexpr It adjacent_find(It f, It l) {
	return loop::adjacent_while(f, l, std::not_equal_to{}).it;
}

// Modifying

template <typename InIt, typename OutIt, typename Fn>
constexpr OutIt copy_if(InIt f, InIt l, OutIt out, Fn fn) {
	auto wr = [fn](auto writer, auto elt) {
		if (fn::bit(fn, elt)) std::invoke(writer, elt);
	};
	return loop::copy_each(f, l, out, wr).out;
}

template <typename InIt, typename OutIt>
constexpr OutIt copy(InIt f, InIt l, OutIt out) {
	return loop::copy_if(f, l, out, fn::constant(true));
}

template <typename InIt, typename OutIt>
constexpr inout<InIt, OutIt> copy_n(InIt f, size_t n, OutIt out) {
	auto writer = [&out](auto elt) {
		*out = elt;
		++out;
	};
	auto g = fn::guard([&n](auto) { return (n-- == 0); }, writer);
	auto in = loop::element_while(f, nullptr, g).it;
	return {in, out};
}

template <typename InIt, typename OutIt, typename Fn>
constexpr OutIt remove_copy_if(InIt f, InIt l, OutIt out, Fn fn) {
	return loop::copy_if(f, l, out, fn::ifnot(fn));
}

template <typename InIt, typename OutIt, typename T>
constexpr OutIt remove_copy(InIt f, InIt l, OutIt out, T val) {
	return loop::remove_copy_if(f, l, out, fn::eq(val));
}

template <typename InIt, typename OutIt, typename Fn>
constexpr OutIt transform(InIt f, InIt l, OutIt out, Fn fn) {
	auto wr = [fn](auto writer, auto elt) {
		std::invoke(writer, std::invoke(fn, elt));
	};
	return loop::copy_each(f, l, out, wr).out;
}

template <typename InIt, typename OutIt, typename Fn, typename T>
constexpr OutIt replace_copy_if(InIt f, InIt l, OutIt out, Fn fn, T anew) {
	auto re = [fn, anew](T elt) { return fn::bit(fn, elt) ? anew : elt; };
	return loop::transform(f, l, out, re);
}

template <typename InIt, typename OutIt, typename T>
constexpr inout<InIt, OutIt> replace_copy(InIt f, InIt l, OutIt out, T prev,
					  T anew) {
	return loop::replace_copy_if(f, l, out, fn::eq(prev), anew);
}

template <typename It, typename Fn>
constexpr void generate(It f, It l, Fn gen) {
	loop::iterator_each(f, l, [gen](auto it) { *it = std::invoke(gen); });
}

template <typename It, typename T>
constexpr void fill(It f, It l, T val) {
	loop::generate(f, l, [val]() { return val; });
}

template <typename It, typename Fn>
constexpr void generate_n(It f, size_t n, Fn gen) {
	auto g = fn::guard([&n](auto) { return (n-- == 0); },
			   [gen](auto it) { *it = std::invoke(gen); });
	loop::iterator_while(f, nullptr, g);
}

template <typename It, typename T>
constexpr void fill_n(It f, size_t n, T val) {
	loop::generate_n(f, n, [val]() { return val; });
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

// Comparision

template <typename It>
constexpr bool equal(It f, It l, It s) {
	auto g = fn::guard([&s](auto elt) { return (elt != *s); },
			   [&s](auto) { ++s; });
	return all_of(f, l, g);
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
