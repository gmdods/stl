#ifndef LOOP_STL_ALGORITHM_HPP
#define LOOP_STL_ALGORITHM_HPP

#include <stdlib.h>
#include <utility>

#include "fn.hpp"
#include "loop.hpp"

namespace loop {

// Non-modifying

template <typename It, typename Fn1>
constexpr void for_each(It f, It l, Fn1 fn1) {
	loop::element_each(f, l, fn1);
}

template <typename It, typename Fn1>
constexpr It for_each_n(It f, size_t n, Fn1 fn1) {
	auto g = fn::guard([&n](auto) { return (n-- == 0); }, fn1);
	return loop::element_while(f, nullptr, g).it;
}

template <typename It, typename If1>
constexpr size_t count_if(It f, It l, If1 if1) {
	size_t count = 0;
	loop::for_each(f, l,
		       [&count, if1](auto elt) { count += fn::bit(if1, elt); });
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return loop::count_if(f, l, fn::eq(val));
}

template <typename It, typename If1>
constexpr bool all_of(It f, It l, If1 if1) {
	return loop::element_while(f, l, if1).ended();
}

template <typename It, typename If1>
constexpr bool none_of(It f, It l, If1 if1) {
	return loop::all_of(f, l, fn::ifnot(if1));
}

template <typename It, typename If1>
constexpr bool any_of(It f, It l, If1 if1) {
	return !loop::none_of(f, l, if1);
}

template <typename It, typename If1>
constexpr It find_if_not(It f, It l, If1 if1) {
	return loop::element_while(f, l, if1).it;
}

template <typename It, typename If1>
constexpr It find_if(It f, It l, If1 if1) {
	return loop::find_if_not(f, l, fn::ifnot(if1));
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return loop::find_if(f, l, fn::eq(val));
}

template <typename ItL, typename ItR>
constexpr std::pair<ItL, ItR> mismatch(ItL f, ItL l, ItR s) {
	auto g = fn::guard([&s](auto elt) { return (elt != *s); },
			   [&s](auto) { ++s; });
	auto it = find_if_not(f, l, g);
	return {it, s};
}

template <typename ItL, typename ItR>
constexpr std::pair<ItL, ItR> mismatch(ItL f, ItL l, ItR s, ItR t) {
	return loop::parallel_while(f, l, s, t, std::equal_to{}).it;
}

template <typename It>
constexpr It adjacent_find(It f, It l) {
	return loop::adjacent_while(f, l, std::not_equal_to{}).it.first;
}

template <typename ItL, typename ItR>
constexpr ItL find_first_of(ItL f, ItL l, ItR s, ItR t) {
	auto if1 = [s, t](auto elt) { return loop::any_of(s, t, fn::eq(elt)); };
	return loop::find_if(f, l, if1);
}

template <typename ItL, typename ItR>
constexpr ItL search(ItL f, ItL l, ItR s, ItR t) {
	auto ret = l;
	auto br1 = [&ret, s, t, l](auto it) {
		auto [l_, t_] = loop::mismatch(it, l, s, t);
		if (t_ == t) {
			ret = it;
			return false;
		}
		return (l_ != l);
	};
	loop::iterator_while(f, nullptr, br1);
	return ret;
}

// Modifying

template <typename InIt, typename OutIt, typename If1>
constexpr OutIt copy_if(InIt f, InIt l, OutIt out, If1 if1) {
	auto wr1 = [if1](auto writer, auto elt) {
		if (fn::bit(if1, elt)) std::invoke(writer, elt);
	};
	return loop::copy_each(f, l, out, wr1);
}

template <typename InIt, typename OutIt>
constexpr OutIt copy(InIt f, InIt l, OutIt out) {
	// return loop::copy_if(f, l, out, fn::constant(true));
	loop::for_each(f, l, fn::writer(out));
	return out;
}

template <typename InIt, typename OutIt>
constexpr inout<InIt, OutIt> copy_n(InIt f, size_t n, OutIt out) {
	auto in = loop::for_each_n(f, n, fn::writer(out));
	return {in, out};
}

template <typename InIt, typename OutIt, typename If1>
constexpr OutIt remove_copy_if(InIt f, InIt l, OutIt out, If1 if1) {
	return loop::copy_if(f, l, out, fn::ifnot(if1));
}

template <typename InIt, typename OutIt, typename T>
constexpr OutIt remove_copy(InIt f, InIt l, OutIt out, T val) {
	return loop::remove_copy_if(f, l, out, fn::eq(val));
}

template <typename InIt, typename OutIt>
constexpr OutIt rotate_copy(InIt f, InIt m, InIt l, OutIt out) {
	out = loop::copy(m, l, out);
	return loop::copy(f, m, out);
}

template <typename InIt, typename OutIt>
constexpr OutIt unique_copy(InIt f, InIt l, OutIt out) {
	if (f == l) return out;
	fn::writer(out)(*f);
	auto wr = [](auto writer, auto lhs, auto rhs) {
		if (rhs != lhs) std::invoke(writer, rhs);
	};
	return loop::copy_adjacent(f, l, out, wr);
}

template <typename InIt, typename OutIt, typename Fn1>
constexpr OutIt transform(InIt f, InIt l, OutIt out, Fn1 fn1) {
	auto wr1 = [fn1](auto writer, auto elt) {
		std::invoke(writer, std::invoke(fn1, elt));
	};
	return loop::copy_each(f, l, out, wr1);
}

template <typename InIt, typename OutIt, typename If1, typename T>
constexpr OutIt replace_copy_if(InIt f, InIt l, OutIt out, If1 if1, T anew) {
	auto re = [if1, anew](T elt) { return fn::bit(if1, elt) ? anew : elt; };
	return loop::transform(f, l, out, re);
}

template <typename InIt, typename OutIt, typename T>
constexpr OutIt replace_copy(InIt f, InIt l, OutIt out, T prev, T anew) {
	return loop::replace_copy_if(f, l, out, fn::eq(prev), anew);
}

template <typename It, typename Fn0>
constexpr void generate(It f, It l, Fn0 gen) {
	loop::iterator_each(f, l, [gen](auto it) { *it = std::invoke(gen); });
}

template <typename It, typename T>
constexpr void fill(It f, It l, T val) {
	loop::generate(f, l, [val]() { return val; });
}

template <typename It, typename Fn0>
constexpr void generate_n(It f, size_t n, Fn0 gen) {
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
	if (f == l) return {l, l};
	It max = f;
	It min = f;
	++f;
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

template <typename ItL, typename ItR>
constexpr bool equal(ItL f, ItL l, ItR s) {
	auto g = fn::guard([&s](auto elt) { return (elt != *s); },
			   [&s](auto) { ++s; });
	return loop::all_of(f, l, g);
}

template <typename ItL, typename ItR>
constexpr bool equal(ItL f, ItL l, ItR s, ItR t) {
	return loop::parallel_while(f, l, s, t, std::equal_to{}).ended();
}

// Partition

template <typename It, typename If1>
constexpr bool is_partitioned(It f, It l, If1 if1) {
	f = loop::find_if_not(f, l, if1);
	return loop::none_of(f, l, if1);
}

template <typename InIt, typename OutItT, typename OutItF, typename If1>
constexpr std::pair<OutItT, OutItF> partition_copy(InIt f, InIt l, OutItT out_t,
						   OutItF out_f, If1 if1) {
	auto wr1 = [if1, writer_f = fn::writer(out_f)](auto writer_t,
						       auto elt) {
		if (fn::bit(if1, elt))
			std::invoke(writer_t, elt);
		else
			std::invoke(writer_f, elt);
	};
	return {loop::copy_each(f, l, out_t, wr1), out_f};
}

template <typename It, typename If1>
constexpr It partition_point(It f, It l, If1 if1) {
	return loop::binary_find(f, l, if1);
}

// Sorts

template <typename It>
constexpr It is_sorted_until(It f, It l) {
	return loop::adjacent_while(f, l, fn::ifnot(std::greater{})).it.second;
}

template <typename It>
constexpr bool is_sorted(It f, It l) {
	return loop::adjacent_while(f, l, fn::ifnot(std::greater{})).ended();
}

// Binary Search

template <typename It, typename T>
constexpr bool binary_search(It f, It l, T val) {
	return loop::binary_recurse(f, l, fn::lt(val), fn::eq(val)).found();
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
	auto ret = loop::binary_recurse(f, l, fn::lt(val), fn::eq(val));
	auto [lb, ub] = ret.it;
	if (ret.found()) {
		auto mid = fn::midpoint(lb, ub);
		return {loop::lower_bound(lb, mid, val),
			loop::upper_bound(mid, ub, val)};
	} else {
		return {lb, ub};
	}
}

} // namespace loop

#endif // !LOOP_STL_ALGORITHM_HPP
