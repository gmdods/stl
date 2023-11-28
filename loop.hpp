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
constexpr exited<It> iterator_while(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!fn::bit(fn, f)) return {f, false};
	}
	return {f, true};
}

template <typename It, typename Fn>
constexpr It iterator_each(It f, It l, Fn fn) {
	return iterator_while(f, l, fn::side_effect(fn)).it;
}

template <typename It, typename Fn>
constexpr exited<It> element_while(It f, It l, Fn fn) {
	return iterator_while(f, l, fn::deref(fn));
}

template <typename It, typename Fn>
constexpr It element_each(It f, It l, Fn fn) {
	return iterator_each(f, l, fn::deref(fn));
}

template <typename T>
constexpr T midpoint(T a, T b) {
	return a + (b - a) / 2;
}

template <typename It, typename Br, typename Fn>
constexpr exited<It> binary(It f, It l, Br br, Fn fn) {
	while (f != l) {
		const auto mid = loop::midpoint(f, l);
		if (!bit(fn, *mid)) return {mid, false};
		if (bit(br, *mid))
			f = mid + 1;
		else
			l = mid;
	}
	return {f, true};
}

template <typename It, typename Br>
constexpr It binary_bound(It f, It l,Br br) {
	return loop::binary(f, l, br, fn::constant(true)).it;
}

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
