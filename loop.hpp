#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include "fn.hpp"

namespace loop {

template <typename It>
struct exited {
	It it;
	bool ended;
	bool found() const { return !ended; }
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

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
