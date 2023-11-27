#include <stdlib.h>

template <typename It, typename Fn>
constexpr It loop_while(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!static_cast<bool>(fn(*f))) break;
	}
	return f;
}

template <typename It, typename Fn>
constexpr It loop_do(It f, It l, Fn fn) {
	return loop_while(f, l, [&fn](auto elt) {
		fn(elt);
		return true;
	});
}

template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	loop_do(f, l, fn);
}

template <typename It, typename Fn>
constexpr size_t count_if(It f, It l, Fn fn) {
	size_t count = 0;
	loop_do(f, l, [&count, &fn](auto elt) {
		count += static_cast<bool>(fn(elt));
	});
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return count_if(f, l, [val](auto elt) { return elt == val; });
}

template <typename It, typename Fn>
constexpr bool all_of(It f, It l, Fn fn) {
	bool all = true;
	loop_while(f, l, [&all, &fn](auto elt) {
		return all &= static_cast<bool>(fn(elt));
	});
	return all;
}

template <typename It, typename Fn>
constexpr bool none_of(It f, It l, Fn fn) {
	return all_of(f, l, [&fn](auto elt) { return !fn(elt); });
}

template <typename It, typename Fn>
constexpr bool any_of(It f, It l, Fn fn) {
	return !none_of(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if_not(It f, It l, Fn fn) {
	return loop_while(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if(It f, It l, Fn fn) {
	return find_if_not(f, l, [&fn](auto elt) { return !fn(elt); });
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return find_if(f, l, [val](auto elt) { return elt == val; });
}
