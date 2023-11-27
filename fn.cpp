#include <functional>

namespace loop {
namespace fn {

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
