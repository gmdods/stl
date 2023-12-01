#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include <functional>
#include <optional>
#include <stdint.h>

#include "fn.hpp"

namespace loop {

template <typename It>
struct range {
	It f, l;
	constexpr operator bool() { return f != l; }
	constexpr auto operator*() { return *f; }
	constexpr void operator++() { ++f; }
};

template <typename Br1>
constexpr auto loop(Br1 br1) {
	for (;;) {
		if (auto r = fn::ret(br1)) return r.value();
	}
}

enum tag : uint8_t { condition = 0, exhaust = 1 };

template <typename It>
struct exited {
	It it;
	loop::tag tag;

	constexpr bool found() const { return tag == tag::condition; }
	constexpr bool ended() const { return tag == tag::exhaust; }
};

template <typename It>
constexpr bool done(It, std::nullptr_t) {
	return false;
}

template <typename It>
constexpr bool done(It f, It l) {
	return f == l;
}

template <typename It, typename St, typename Br1>
constexpr exited<It> iterator_while(It f, St l, Br1 br1) {
	auto ret = loop::loop([&f, l, br1]() -> std::optional<loop::tag> {
		if (loop::done(f, l)) return tag::exhaust;
		if (!fn::bit(br1, f)) return tag::condition;
		++f;
		return std::nullopt;
	});
	return {f, ret};
}

template <typename It, typename Fn1>
constexpr It iterator_each(It f, It l, Fn1 fn1) {
	return loop::iterator_while(f, l, fn::side_effect(fn1)).it;
}

template <typename It, typename St, typename Br1>
constexpr exited<It> element_while(It f, St l, Br1 br1) {
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

template <typename It, typename St, typename Br2>
constexpr exited<It> adjacent_while(It f, St l, Br2 br2) {
	if (loop::done(f, l)) return {f, tag::exhaust};
	It t = f;
	++f;
	auto ret = loop::loop([&f, &t, l, br2]() -> std::optional<loop::tag> {
		if (loop::done(f, l)) return tag::exhaust;
		if (!fn::bit(br2, *t, *f)) return tag::condition;
		t = f;
		++f;
		return std::nullopt;
	});
	return {t, ret};
}

template <typename ItL, typename ItR, typename Br2>
constexpr exited<std::pair<ItL, ItR>> parallel_while(ItL f, ItL l, ItR s, ItR t,
						     Br2 br2) {
	auto ret =
	    loop::loop([&f, l, &s, t, br2]() -> std::optional<loop::tag> {
		    if (loop::done(f, l)) return tag::exhaust;
		    if (loop::done(s, t)) return tag::exhaust;
		    if (!fn::bit(br2, *f, *s)) return tag::condition;
		    (void) ++f, (void) ++s;
		    return std::nullopt;
	    });
	return {{f, s}, ret};
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
		if (fn::bit(br1, *mid)) return {{f, l}, tag::condition};
		if (fn::bit(if1, *mid))
			f = mid + 1;
		else
			l = mid;
	}
	return {{f, l}, tag::exhaust};
}

template <typename It, typename If1>
constexpr It binary_find(It f, It l, If1 if1) {
	auto [lb, ub] = loop::binary_recurse(f, l, if1, fn::constant(false)).it;
	return fn::midpoint(lb, ub);
}

} // namespace loop

#endif // !LOOP_STL_LOOP_HPP
