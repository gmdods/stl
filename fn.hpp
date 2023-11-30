#ifndef LOOP_STL_FN_HPP
#define LOOP_STL_FN_HPP

#include <functional>

namespace loop {

namespace fn {

/* -----------------------
 * Function naming
 *
 * - Fn1 : Unary function
 * - Fn2 : Binary function
 *
 * - If1 : Unary predicate
 * - If2 : Binary predicate
 *
 * - Br1 : Unary break condition
 * - Br2 : Binary break condition
 *
 * - Wr1 : Unary writer
 * - Wr2 : Binary writer
 * ----------------------
 */

template <typename T>
constexpr T midpoint(T a, T b) {
	return a + (b - a) / 2;
}

template <typename Fn1>
constexpr auto deref(Fn1 fn1) {
	return [fn1](auto it) { return std::invoke(fn1, *it); };
};

template <typename Fn1>
constexpr auto side_effect(Fn1 fn1) {
	return [fn1](auto elt) {
		std::invoke(fn1, elt);
		return true;
	};
};

template <typename OutIt>
constexpr auto writer(OutIt & out) {
	return [&out](auto elt) {
		*out = elt;
		++out;
	};
};

template <typename Fn2, typename Fn1>
constexpr auto before(Fn2 fn2, Fn1 fn1) {
	return [fn2, fn1](auto lhs, auto rhs) {
		return std::invoke(fn2, lhs, std::invoke(fn1, rhs));
	};
}

template <typename T>
struct constant {
	T val;
	constant(T val) : val(val) {}

	template <typename U>
	constexpr bool operator()(U) const {
		return val;
	}
};

template <typename If, typename... Ts>
constexpr bool bit(If if_, Ts... elt) {
	return static_cast<bool>(std::invoke(if_, elt...));
}

template <typename If1>
struct ifnot {
	If1 if1;
	ifnot(If1 if1) : if1(if1) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		return !bit(if1, elt);
	}
};

template <typename If1, typename Fn1>
struct guard {
	If1 if1;
	Fn1 fn1;
	guard(If1 if1, Fn1 fn1) : if1(if1), fn1(fn1) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		if (fn::bit(if1, elt))
			return false;
		else {
			std::invoke(fn1, elt);
			return true;
		}
	}
};

template <typename T>
struct eq {
	T val;
	eq(T val) : val(val) {}
	constexpr bool operator()(T elt) const { return elt == val; }
};

template <typename T>
struct lt {
	T val;
	lt(T val) : val(val) {}
	constexpr bool operator()(T elt) const { return elt < val; }
};

template <typename T>
struct gt {
	T val;
	gt(T val) : val(val) {}
	constexpr bool operator()(T elt) const { return val < elt; }
};

} // namespace fn
} // namespace loop

#endif // !LOOP_STL_FN_HPP
