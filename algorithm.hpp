#ifndef LOOP_STL_ALGORITHM_HPP
#define LOOP_STL_ALGORITHM_HPP

#include <stdlib.h>

#include "fn.hpp"
#include "loop.hpp"

namespace loop {

// Non-modifying

template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	loop::elt_do(f, l, fn);
}

template <typename It, typename Fn>
constexpr size_t count_if(It f, It l, Fn fn) {
	size_t count = 0;
	loop::elt_do(f, l, [&count, fn](auto elt) { count += fn::bit(fn, elt); });
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return loop::count_if(f, l, fn::eq(val));
}

template <typename It, typename Fn>
constexpr bool all_of(It f, It l, Fn fn) {
	return loop::elt_while(f, l, fn).ended;
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
	return loop::elt_while(f, l, fn).it;
}

template <typename It, typename Fn>
constexpr It find_if(It f, It l, Fn fn) {
	return loop::find_if_not(f, l, fn::ifnot(fn));
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return loop::find_if(f, l, fn::eq(val));
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
	loop::iter_do(f, l, [&max, &min](auto it) {
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

} // namespace loop

#endif // !LOOP_STL_ALGORITHM_HPP
