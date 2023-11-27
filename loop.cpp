namespace loop {

template <typename It, typename Fn>
constexpr It while_(It f, It l, Fn fn) {
	for (; f != l; ++f) {
		if (!static_cast<bool>(fn(*f))) break;
	}
	return f;
}

template <typename It, typename Fn>
constexpr It do_(It f, It l, Fn fn) {
	return while_(f, l, [&fn](auto elt) {
		fn(elt);
		return true;
	});
}

} // namespace loop
