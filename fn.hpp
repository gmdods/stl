#ifndef LOOP_STL_FN_HPP
#define LOOP_STL_FN_HPP

#include <functional>

namespace loop {

namespace fn {

template <typename Fn>
struct deref {
	Fn f;
	deref(Fn f) : f(f) {}

	template <typename It>
	constexpr auto operator()(It it) const {
		return std::invoke(f, *it);
	}
};

template <typename Fn>
struct side_effect {
	Fn fn;
	side_effect(Fn fn) : fn(fn) {}

	template <typename... Ts>
	constexpr bool operator()(Ts... elt) const {
		std::invoke(fn, elt...);
		return true;
	}
};

template <typename OutIt>
auto writer(OutIt & out) {
	return [&out](auto elt) {
		*out = elt;
		++out;
	};
};

template <typename T>
struct constant {
	T val;
	constant(T val) : val(val) {}

	template <typename U>
	constexpr bool operator()(U) const {
		return val;
	}
};

template <typename Fn, typename... Ts>
constexpr bool bit(Fn f, Ts... elt) {
	return static_cast<bool>(std::invoke(f, elt...));
}

template <typename Fn>
struct ifnot {
	Fn f;
	ifnot(Fn f) : f(f) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		return !bit(f, elt);
	}
};

template <typename Br, typename Fn>
struct guard {
	Br br;
	Fn fn;
	guard(Br br, Fn fn) : br(br), fn(fn) {}

	template <typename T>
	constexpr bool operator()(T elt) const {
		if (fn::bit(br, elt))
			return false;
		else {
			std::invoke(fn, elt);
			return true;
		}
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
