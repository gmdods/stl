#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include "fn.hpp"

namespace loop {

template <typename T>
constexpr T midpoint(T a, T b) {
	return a + (b - a) / 2;
}

template <typename It>
struct exited {
	It it;
	bool ended_;

	constexpr bool ended() const { return ended_; }
	constexpr bool found() const { return !ended_; }
};

template <typename It>
struct range {
	It f, l;
	constexpr operator bool() { return f != l; }
	constexpr auto operator*() { return *f; }
	constexpr void operator++() { ++f; }
};

template <typename It, typename Fn>
constexpr exited<It> iterator_while(It f, std::nullptr_t, Fn fn) {
	for (;; ++f) {
		if (!fn::bit(fn, f)) return {f, false};
	}
	return {f, true};
}

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
constexpr exited<It> element_while(It f, std::nullptr_t, Fn fn) {
	return iterator_while(f, nullptr, fn::deref(fn));
}

template <typename It, typename Fn>
constexpr exited<It> element_while(It f, It l, Fn fn) {
	return iterator_while(f, l, fn::deref(fn));
}

template <typename It, typename Fn>
constexpr It element_each(It f, It l, Fn fn) {
	return iterator_each(f, l, fn::deref(fn));
}

template <typename InIt, typename OutIt>
struct inout {
	InIt in;
	OutIt out;
};

template <typename InIt, typename OutIt, typename FnW>
constexpr inout<InIt, OutIt> copy_each(InIt f, InIt l, OutIt out, FnW fn) {
	auto in = element_each(f, l, [fn, writer = fn::writer(out)](auto elt) {
		std::invoke(fn, writer, elt);
	});
	return {in, out};
}

template <typename It, typename Fn>
constexpr exited<It> adjacent_while(It f, It l, Fn fn) {
	It t = f;
	if (f != l) {
		++f;
		for (; f != l; ++f) {
			if (!fn::bit(fn, *t, *f)) return {t, false};
			t = f;
		}
	}
	return {t, true};
}

template <typename It, typename Br, typename Fn>
constexpr exited<range<It>> binary(It f, It l, Br br, Fn fn) {
	while (f != l) {
		const auto mid = loop::midpoint(f, l);
		if (!fn::bit(fn, *mid)) return {{f, l}, false};
		if (fn::bit(br, *mid))
			f = mid + 1;
		else
			l = mid;
	}
	return {{f, l}, true};
}

template <typename It, typename Br>
constexpr It binary_bound(It f, It l, Br br) {
	auto [lb, ub] = loop::binary(f, l, br, fn::constant(true)).it;
	return loop::midpoint(lb, ub);
}

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
