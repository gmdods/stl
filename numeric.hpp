#ifndef LOOP_STL_NUMERIC_HPP
#define LOOP_STL_NUMERIC_HPP

#include <stdlib.h>

#include "algorithm.hpp"
#include "fn.hpp"
#include "loop.hpp"

namespace loop {

template <typename It, typename T>
constexpr void iota(It f, It l, T val) {
	loop::generate(f, l, [&val]() { return val++; });
}

template <typename It, typename T, typename Fn2>
constexpr T accumulate(It f, It l, T init, Fn2 fn2) {
	T acc = init;
	loop::for_each(
	    f, l, [&acc, fn2](auto elt) { acc = std::invoke(fn2, acc, elt); });
	return acc;
}

template <typename It, typename T, typename Fn2>
constexpr T reduce(It f, It l, T init, Fn2 fn2) {
	return loop::accumulate(f, l, init, fn2);
}

template <typename It, typename T, typename Fn2, typename Fn1>
constexpr T transform_reduce(It f, It l, T init, Fn2 fn2, Fn1 fn1) {
	return loop::reduce(f, l, init, fn::before(fn2, fn1));
}

template <typename ItL, typename ItR, typename T, typename Fn2R, typename Fn2M>
constexpr T inner_product(ItL f, ItL l, ItR s, T init, Fn2R fn2_r, Fn2M fn2_m) {
	auto fn1 = [&s, fn2_m](auto elt) {
		auto ret = std::invoke(fn2_m, elt, *s);
		++s;
		return ret;
	};
	return loop::transform_reduce(f, l, init, fn2_r, fn1);
}

template <typename InIt, typename OutIt, typename Fn2>
constexpr OutIt adjancent_difference(InIt f, InIt l, OutIt out, Fn2 fn2) {
	if (f == l) return out;
	fn::writer(out)(*f);
	auto wr2 = [fn2](auto writer, auto lhs, auto rhs) {
		std::invoke(writer, std::invoke(fn2, rhs, lhs));
	};
	return loop::copy_adjacent(f, l, out, wr2);
}

template <typename InIt, typename OutIt, typename T, typename Fn2>
constexpr OutIt inclusive_scan(InIt f, InIt l, OutIt out, T val, Fn2 fn2) {
	T acc = val;
	auto wr1 = [&acc, fn2](auto writer, auto elt) {
		acc = std::invoke(fn2, acc, elt);
		std::invoke(writer, acc);
	};
	return loop::copy_each(f, l, out, wr1);
}

template <typename InIt, typename OutIt, typename T, typename Fn2>
constexpr OutIt exclusive_scan(InIt f, InIt l, OutIt out, T val, Fn2 fn2) {
	T acc = val;
	auto wr1 = [&acc, fn2](auto writer, auto elt) {
		std::invoke(writer, acc);
		acc = std::invoke(fn2, acc, elt);
	};
	return loop::copy_each(f, l, out, wr1);
}

template <typename InIt, typename OutIt, typename Fn2>
constexpr OutIt partial_sum(InIt f, InIt l, OutIt out, Fn2 fn2) {
	if (f == l) return out;
	auto init = *f;
	fn::writer(out)(init);
	++f;
	return loop::inclusive_scan(f, l, out, init, fn2);
}

template <typename InIt, typename OutIt, typename T, typename Fn2, typename Fn1>
constexpr OutIt transform_inclusive_scan(InIt f, InIt l, OutIt out, T init,
					 Fn2 fn2, Fn1 fn1) {
	return loop::inclusive_scan(f, l, out, init, fn::before(fn2, fn1));
}

template <typename InIt, typename OutIt, typename T, typename Fn2, typename Fn1>
constexpr OutIt transform_exclusive_scan(InIt f, InIt l, OutIt out, T init,
					 Fn2 fn2, Fn1 fn1) {
	return loop::exclusive_scan(f, l, out, init, fn::before(fn2, fn1));
}

} // namespace loop

#endif // !LOOP_STL_NUMERIC_HPP
