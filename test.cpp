#include <array>
#include <cassert>

#include "algorithm.cpp"

int main() {
	auto odd = [](auto i) { return static_cast<bool>(i % 2); };

	std::array a{1, 2, 3, 4, 5, 6};

	assert(all_of(a.cbegin(), a.cend(), [](auto i) { return i <= 6; }));
	assert(any_of(a.cbegin(), a.cend(), [](auto i) { return i == 3; }));
	assert(none_of(a.cbegin(), a.cend(), [](auto i) { return i > 6; }));

	size_t sum = 0;
	for_each(a.cbegin(), a.cend(), [&sum](auto i) { sum += i; });
	assert(21 == sum);

	assert(3 == count_if(a.cbegin(), a.cend(), odd));
	assert(1 == count(a.cbegin(), a.cend(), 3));

	assert(a.begin() == find_if(a.cbegin(), a.cend(), odd));
	assert(3 == *find(a.cbegin(), a.cend(), 3));

	return 0;
}
