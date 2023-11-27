#include <stdlib.h>

#include "loop.cpp"
#include "fn.cpp"

namespace loop {

template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	do_(f, l, fn);
}

template <typename It, typename Fn>
constexpr size_t count_if(It f, It l, Fn fn) {
	size_t count = 0;
	do_(f, l,
	    [&count, &fn](auto elt) { count += static_cast<bool>(fn(elt)); });
	return count;
}

template <typename It, typename T>
constexpr size_t count(It f, It l, T val) {
	return count_if(f, l, fn::eq(val));
}

template <typename It, typename Fn>
constexpr bool all_of(It f, It l, Fn fn) {
	bool all = true;
	while_(f, l, [&all, &fn](auto elt) {
		return all = static_cast<bool>(fn(elt));
	});
	return all;
}

template <typename It, typename Fn>
constexpr bool none_of(It f, It l, Fn fn) {
	return all_of(f, l, fn::ifnot(fn));
}

template <typename It, typename Fn>
constexpr bool any_of(It f, It l, Fn fn) {
	return !none_of(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if_not(It f, It l, Fn fn) {
	return while_(f, l, fn);
}

template <typename It, typename Fn>
constexpr It find_if(It f, It l, Fn fn) {
	return find_if_not(f, l, fn::ifnot(fn));
}

template <typename It, typename T>
constexpr It find(It f, It l, T val) {
	return find_if(f, l, fn::eq(val));
}

} // namespace loop
