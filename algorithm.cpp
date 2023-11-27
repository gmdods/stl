#include <stdlib.h>

template <typename It, typename Fn>
constexpr void loop_while(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!fn(*f)) break;
	}
}

template <typename It, typename Fn>
constexpr void loop_do(It f, It l, Fn fn) {
	loop_while(f, l, [&fn](auto elt) {
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
