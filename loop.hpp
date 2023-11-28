#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include "fn.hpp"

namespace loop {

template <typename It>
struct exited {
	It it;
	bool ended_;
	constexpr bool ended() const { return ended_; }
	constexpr bool found() const { return !ended_; }
};

template <typename It, typename Fn>
constexpr exited<It> iter_while(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!fn::bit(fn, f)) return {f, false};
	}
	return {f, true};
}

template <typename It, typename Fn>
constexpr It iter_do(It f, It l, Fn fn) {
	return iter_while(f, l, fn::side_effect(fn)).it;
}

template <typename It, typename Fn>
constexpr exited<It> elt_while(It f, It l, Fn fn) {
	return iter_while(f, l, fn::deref(fn));
}

template <typename It, typename Fn>
constexpr It elt_do(It f, It l, Fn fn) {
	return iter_do(f, l, fn::deref(fn));
}

template <typename It, typename Br, typename Fn>
constexpr exited<It> binary(It f, It l, Br br, Fn fn) {
	It lb = f;
	It ub = l;
	while (lb != ub) {
		const auto mid = lb + (ub - lb) / 2;
		if (!bit(fn, *mid)) return {mid, false};
		if (bit(br, *mid))
			lb = mid + 1;
		else
			ub = mid;
	}
	return {lb, true};
}

template <typename It, typename Fn>
constexpr It binary_bound(It f, It l, Fn fn) {
	return loop::binary(f, l, fn, fn::constant(true)).it;
}

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
