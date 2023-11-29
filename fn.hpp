#ifndef LOOP_STL_FN_HPP
#define LOOP_STL_FN_HPP

#include <functional>

namespace loop {

namespace fn {

template <typename Fn>
constexpr auto deref(Fn fn) {
	return [fn](auto it) { return std::invoke(fn, *it); };
};

template <typename Fn>
constexpr auto side_effect(Fn fn) {
	return [fn](auto elt) {
		std::invoke(fn, elt);
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

template <typename Bin, typename Fn>
constexpr auto before(Bin bin, Fn fn) {
	return [bin, fn](auto lhs, auto rhs) {
		return std::invoke(bin, lhs, std::invoke(fn, rhs));
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
