#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include <functional>

#include "fn.hpp"

namespace loop {

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

template <typename It, typename Br1>
constexpr exited<It> iterator_while(It f, std::nullptr_t, Br1 br1) {
	for (;; ++f) {
		if (!fn::bit(br1, f)) return {f, false};
	}
	return {f, true};
}

template <typename It, typename Br1>
constexpr exited<It> iterator_while(It f, It l, Br1 br1) {
	for (; f != l; ++f) {
		if (!fn::bit(br1, f)) return {f, false};
	}
	return {f, true};
}

template <typename It, typename Fn1>
constexpr It iterator_each(It f, It l, Fn1 fn1) {
	return loop::iterator_while(f, l, fn::side_effect(fn1)).it;
}

template <typename It, typename Br1>
constexpr exited<It> element_while(It f, std::nullptr_t, Br1 br1) {
	return loop::iterator_while(f, nullptr, fn::deref(br1));
}

template <typename It, typename Br1>
constexpr exited<It> element_while(It f, It l, Br1 br1) {
	return loop::iterator_while(f, l, fn::deref(br1));
}

template <typename It, typename Fn1>
constexpr It element_each(It f, It l, Fn1 fn1) {
	return loop::iterator_each(f, l, fn::deref(fn1));
}

template <typename InIt, typename OutIt>
struct inout {
	InIt in;
	OutIt out;
};

template <typename InIt, typename OutIt, typename Wr1>
constexpr inout<InIt, OutIt> copy_each(InIt f, InIt l, OutIt out, Wr1 wr1) {
	auto fn1 = [wr1, writer = fn::writer(out)](auto elt) {
		std::invoke(wr1, writer, elt);
	};
	auto in = loop::element_each(f, l, fn1);
	return {in, out};
}

template <typename It, typename Br2>
constexpr exited<It> adjacent_while(It f, It l, Br2 br2) {
	It t = f;
	if (f != l) {
		++f;
		for (; f != l; ++f) {
			if (!fn::bit(br2, *t, *f)) return {t, false};
			t = f;
		}
	}
	return {t, true};
}

template <typename InIt, typename OutIt, typename Wr2>
constexpr inout<InIt, OutIt> copy_adjacent(InIt f, InIt l, OutIt out, Wr2 wr2) {
	auto br2 = [wr2, writer = fn::writer(out)](auto lhs, auto rhs) {
		std::invoke(wr2, writer, lhs, rhs);
		return true;
	};
	auto in = loop::adjacent_while(f, l, br2).it;
	return {in, out};
}

template <typename It, typename If1, typename Br1>
constexpr exited<range<It>> binary_recurse(It f, It l, If1 if1, Br1 br1) {
	while (f != l) {
		const auto mid = fn::midpoint(f, l);
		if (fn::bit(br1, *mid)) return {{f, l}, false};
		if (fn::bit(if1, *mid))
			f = mid + 1;
		else
			l = mid;
	}
	return {{f, l}, true};
}

template <typename It, typename If1>
constexpr It binary_find(It f, It l, If1 if1) {
	auto [lb, ub] = loop::binary_recurse(f, l, if1, fn::constant(false)).it;
	return fn::midpoint(lb, ub);
}

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
