#include "fn.cpp"

namespace loop {

template <typename It, typename Fn>
constexpr It iter_while(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!static_cast<bool>(fn(f))) break;
	}
	return f;
}

template <typename It, typename Fn>
constexpr It iter_do(It f, It l, Fn fn) {
	return iter_while(f, l, [&fn](auto elt) {
		fn(elt);
		return true;
	});
}

template <typename It, typename Fn>
constexpr It elt_while(It f, It l, Fn fn) {
	return iter_while(f, l, fn::deref(fn));
}

template <typename It, typename Fn>
constexpr It elt_do(It f, It l, Fn fn) {
	return iter_do(f, l, fn::deref(fn));
}

} // namespace loop
