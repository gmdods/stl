#ifndef LOOP_STL_LOOP_HPP
#define LOOP_STL_LOOP_HPP

#include <functional>
#include <optional>
#include <stdint.h>

#include "fn.hpp"

namespace loop {

template <typename It>
constexpr bool done(It, std::nullptr_t) {
	return false;
}

template <typename It>
constexpr bool done(It f, It l) {
	return f == l;
}

template <typename It, typename St = It>
struct range {
	It f;
	St l;
	constexpr explicit operator bool() { return !loop::done(f, l); }
	constexpr auto it() { return f; }
	constexpr auto operator*() { return *f; }
	constexpr void operator++() { ++f; }

	constexpr bool operator==(range<It, St> o) {
		return (f == o.f) && (l == o.l);
	}
};

template <typename It, typename St>
struct adjacent_range {
	It t;
	It f;
	St l;
	constexpr explicit operator bool() { return !loop::done(f, l); }
	constexpr auto it() { return t; }
	constexpr auto operator*() { return std::pair{*t, *f}; }
	constexpr void operator++() {
		t = f;
		++f;
	}
};

template <typename It0, typename St0, typename It1, typename St1>
struct parallel_range {
	It0 f;
	St0 l;
	It1 s;
	St1 t;
	constexpr explicit operator bool() {
		return !loop::done(f, l) && !loop::done(s, t);
	}
	constexpr auto it() { return std::pair{f, s}; }
	constexpr auto operator*() { return std::pair{*f, *s}; }
	constexpr void operator++() { (void) ++f, (void) ++s; }
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

template <typename Rng, typename Br1>
constexpr exited<Rng> range_while(Rng r, Br1 br1) {
	for (; bool(r); ++r) {
		if (!fn::bit(br1, r)) return {r, tag::condition};
	}
	return {r, tag::exhaust};
}

template <typename It, typename St, typename Br1>
constexpr exited<It> iterator_while(It f, St l, Br1 br1) {
	auto [r, ret] = loop::range_while(
	    loop::range{f, l}, [br1](auto r) { return fn::bit(br1, r.f); });
	return {r.it(), ret};
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

	auto [r, ret] = loop::range_while(loop::adjacent_range{t, f, l},
					  fn::deref(fn::unpair(br2)));
	return {r.it(), ret};
}

template <typename ItL, typename ItR, typename Br2>
constexpr exited<std::pair<ItL, ItR>> parallel_while(ItL f, ItL l, ItR s, ItR t,
						     Br2 br2) {
	auto [r, ret] = loop::range_while(loop::parallel_range{f, l, s, t},
					  fn::deref(fn::unpair(br2)));
	return {r.it(), ret};
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
