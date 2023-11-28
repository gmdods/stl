#ifndef LOOP_STL_NUMERIC_HPP
#define LOOP_STL_NUMERIC_HPP

#include <stdlib.h>

#include "fn.hpp"
#include "loop.hpp"

namespace loop {

template <typename It, typename T, typename Bin>
constexpr T accumulate(It f, It l, T init, Bin bin) {
	T acc = init;
	elt_do(f, l,
	       [&acc, bin](auto elt) { acc = std::invoke(bin, acc, elt); });
	return acc;
}

template <typename It, typename T, typename Bin>
constexpr T reduce(It f, It l, T init, Bin bin) {
	return accumulate(f, l, init, bin);
}

template <typename It, typename T, typename Bin, typename Fn>
constexpr T transform_reduce(It f, It l, T init, Bin bin, Fn fn) {
	return reduce(f, l, init, fn::rhs(bin, fn));
}

} // namespace loop

#endif // !LOOP_STL_NUMERIC_HPP
