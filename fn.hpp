#ifndef LOOP_STL_FN_HPP
#define LOOP_STL_FN_HPP

#include <functional>

namespace loop {
namespace fn {

template <typename Fn>
struct deref {
	Fn f;
	deref(Fn & f) : f(f) {}

	template <typename It>
	constexpr auto operator()(It it) const {
		return std::invoke(f, *it);
	}
};

template <typename Fn, typename T>
constexpr bool bit(Fn f, T elt) {
	return static_cast<bool>(std::invoke(f, elt));
}

template <typename Fn>
struct ifnot {
	Fn f;
	ifnot(Fn & f) : f(f) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		return !bit(f, elt);
	}
};

template <typename Fn>
struct side_effect {
	Fn fn;
	side_effect(Fn fn) : fn(fn) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		std::invoke(fn, elt);
		return true;
	}
};

template <typename Bin, typename Fn>
struct rhs {
	Bin bin;
	Fn fn;
	rhs(Bin bin, Fn fn) : bin(bin), fn(fn) {}

	template <typename T>
	constexpr T operator()(T lhs_, T rhs_) const {
		return std::invoke(bin, lhs_, std::invoke(fn, rhs_));
	}
};

template <typename T>
struct eq {
	T val;
	eq(T val) : val(val) {}

	constexpr bool operator()(T elt) const { return elt == val; }
};

} // namespace fn
} // namespace loop

#endif // !LOOP_STL_FN_HPP
