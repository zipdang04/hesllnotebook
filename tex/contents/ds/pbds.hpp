#pragma once
#include "../contest/template.hpp"

// pb_ds. OrderedSet / OrderedMap: ranks from 0, O(log n).
// gp_hash_table + CustomHash (splitmix64): O(1) expected.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template<class T> using OrderedSet = tree<T, null_type,
	less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<class K, class V> using OrderedMap = tree<K, V,
	less<K>, rb_tree_tag, tree_order_statistics_node_update>;
struct CustomHash{
	size_t operator()(uint64_t x) const{
		// random per run, so inputs cannot target it
		static const uint64_t seed = CURRENT_TIMESTAMP();
		x += seed + 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
};
