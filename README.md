# STL - Standard Template Library
An implementation of C++ STL

`<numeric>` [cppreference](https://en.cppreference.com/w/cpp/header/numeric)

- **Functions**
	- [x] iota
	- [x] accumulate
	- [x] reduce
	- [x] transform_reduce
	- [x] inner_product
	- [x] adjacent_difference
	- [x] partial_sum
	- [x] inclusive_scan
	- [x] exclusive_scan
	- [x] transform_inclusive_scan
	- [x] transform_exclusive_scan

`<algorithm>` [cppreference](https://en.cppreference.com/w/cpp/header/algorithm)

- **Non-modifying sequence operations**
	- [x] `all_of` `any_of` `none_of`
	- [x] `for_each`
	- [x] `for_each_n`
	- [x] `count` `count_if`
	- [x] `mismatch`
	- [x] `find` `find_if` `find_if_not`
	- [ ] `find_end`
	- [x] `find_first_of`
	- [x] `adjacent_find`
	- [x] `search`
	- [ ] `search_n`
- **Modifying sequence operations**
    - [x] `copy` `copy_if`
    - [x] `copy_n`
    - [ ] `copy_backward`
	- [ ] `move`
	- [ ] `move_backward`
	- [x] `fill`
	- [x] `fill_n`
	- [x] `transform`
	- [x] `generate`
	- [x] `generate_n`
	- [ ] `remove` `remove_if`
	- [x] `remove_copy` `remove_copy_if`
	- [ ] `replace` `replace_if`
	- [x] `replace_copy` `replace_copy_if`
	- [ ] `swap`
	- [ ] `iter_swap`
	- [ ] `reverse`
	- [ ] `reverse_copy`
	- [ ] `rotate`
	- [x] `rotate_copy`
	- [ ] `shift_left` `shift_right`
	- [ ] `random_shuffle` `shuffle`
	- [ ] `sample`
	- [ ] `unique`
	- [x] `unique_copy`
- **Partitioning operations**
	- [x] `is_partitioned`
	- [ ] `partition`
	- [x] `partition_copy`
	- [ ] `stable_partition`
	- [x] `partition_point`
- **Sorting operations**
	- [x] `is_sorted`
	- [x] `is_sorted_until`
	- [ ] `sort`
	- [ ] `partial_sort`
	- [ ] `partial_sort_copy`
	- [ ] `stable_sort`
	- [ ] `nth_element`
- **Binary search operations (on sorted ranges)**
	- [x] `lower_bound`
	- [x] `upper_bound`
	- [x] `binary_search`
	- [x] `equal_range`
- **Modifying operations (on sorted ranges)**
	- [ ] `merge`
	- [ ] `inplace_merge`
- **Set operations (on sorted ranges)**
	- [ ] `includes`
	- [ ] `set_difference`
	- [ ] `set_intersection`
	- [ ] `set_symmetric_difference`
	- [ ] `set_union`
- **Heap operations**
	- [ ] `is_heap`
	- [ ] `is_heap_until`
	- [ ] `make_heap`
	- [ ] `push_heap`
	- [ ] `pop_heap`
	- [ ] `sort_heap`
- **Minimum/maximum operations**
	- [x] `max_element`
	- [x] `min_element`
	- [x] `minmax_element`
- **Comparison operations**
	- [x] `equal`
	- [ ] `lexicographical_compare`
	- [ ] `lexicographical_compare_three_way`
- **Permutation operations**
	- [ ] `is_permutation`
	- [ ] `next_permutation`
	- [ ] `prev_permutation`

## Testing

Testing uses [c-unittest](https://github.com/gmdods/c-unittest).

