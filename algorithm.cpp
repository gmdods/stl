template <typename It, typename Fn>
constexpr void for_each(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		fn(*f);
	}
}
