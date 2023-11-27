#ifndef LOOP_STL_HPP
#define LOOP_STL_HPP

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

template <typename Fn>
struct ifnot {
	Fn f;
	ifnot(Fn & f) : f(f) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		return !std::invoke(f, elt);
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

#endif // !LOOP_STL_HPP
