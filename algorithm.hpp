#ifndef LOOP_STL_ALGORITHM_HPP
#define LOOP_STL_ALGORITHM_HPP

#include <stdlib.h>

#include "fn.hpp"
#include "loop.hpp"

namespace loop {

template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	elt_do(f, l, fn);
}

template <typename It, typename Fn>
constexpr size_t count_if(It f, It l, Fn fn) {
	size_t count = 0;
	elt_do(f, l, [&count, &fn](auto elt) {
		count += static_cast<bool>(fn(elt));
	});
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return count_if(f, l, fn::eq(val));
}

template <typename It, typename Fn>
constexpr bool all_of(It f, It l, Fn fn) {
	bool all = true;
	elt_while(f, l, [&all, &fn](auto elt) {
		return all = static_cast<bool>(fn(elt));
	});
	return all;
}

template <typename It, typename Fn>
constexpr bool none_of(It f, It l, Fn fn) {
	return all_of(f, l, fn::ifnot(fn));
}

template <typename It, typename Fn>
constexpr bool any_of(It f, It l, Fn fn) {
	return !none_of(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if_not(It f, It l, Fn fn) {
	return elt_while(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if(It f, It l, Fn fn) {
	return find_if_not(f, l, fn::ifnot(fn));
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return find_if(f, l, fn::eq(val));
}

template <typename It>
struct minmax {
	It min, max;
};

template <typename It>
constexpr minmax<It> minmax_element(It f, It l) {
	It max = f;
	It min = f;
	iter_do(f, l, [&max, &min](auto it) {
		if (*it > *max) max = it;
		if (*it < *min) min = it;
	});
	return {min, max};
}

template <typename It>
constexpr It max_element(It f, It l) {
	return minmax_element(f, l).max;
}

template <typename It>
constexpr It min_element(It f, It l) {
	return minmax_element(f, l).min;
}

} // namespace loop

#endif // !LOOP_STL_ALGORITHM_HPP
