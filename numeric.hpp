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

template <typename It, typename T, typename Bin>
constexpr T accumulate(It f, It l, T init, Bin bin) {
	T acc = init;
	loop::for_each(
	    f, l, [&acc, bin](auto elt) { acc = std::invoke(bin, acc, elt); });
	return acc;
}

template <typename It, typename T, typename Bin>
constexpr T reduce(It f, It l, T init, Bin bin) {
	return loop::accumulate(f, l, init, bin);
}

template <typename It, typename T, typename Bin, typename Fn>
constexpr T transform_reduce(It f, It l, T init, Bin bin, Fn fn) {
	return loop::reduce(f, l, init, fn::rhs(bin, fn));
}

template <typename It, typename T, typename BinR, typename BinM>
constexpr T inner_product(It f, It l, It s, T init, BinR bin_r, BinM bin_m) {
	return loop::reduce(f, l, init, [&s, bin_m, bin_r](auto acc, auto elt) {
		auto ret = std::invoke(bin_r, acc, std::invoke(bin_m, elt, *s));
		++s;
		return ret;
	});
}

template <typename InIt, typename OutIt, typename T, typename Bin>
constexpr OutIt inclusive_scan(InIt f, InIt l, OutIt out, T val, Bin bin) {
	T acc = val;
	auto wr = [&acc, bin](auto writer, auto elt) {
		acc = std::invoke(bin, acc, elt);
		std::invoke(writer, acc);
	};
	return loop::copy_each(f, l, out, wr).out;
}

template <typename InIt, typename OutIt, typename T, typename Bin>
constexpr OutIt exclusive_scan(InIt f, InIt l, OutIt out, T val, Bin bin) {
	T acc = val;
	auto wr = [&acc, bin](auto writer, auto elt) {
		std::invoke(writer, acc);
		acc = std::invoke(bin, acc, elt);
	};
	return loop::copy_each(f, l, out, wr).out;
}

template <typename InIt, typename OutIt, typename Bin>
constexpr OutIt partial_sum(InIt f, InIt l, OutIt out, Bin bin) {
	if (f == l) return out;
	auto init = *f;
	fn::writer(out)(init);
	++f;
	return loop::inclusive_scan(f, l, out, init, bin);
}

} // namespace loop

#endif // !LOOP_STL_NUMERIC_HPP
