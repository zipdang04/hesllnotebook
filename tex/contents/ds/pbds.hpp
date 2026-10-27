#pragma once
#include "../contest/template.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

// set + find_by_order(k) (k-th smallest, 0-indexed; end() if k >= size), order_of_key(x) (# < x)
template<class T> using ordSet =
	tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

struct chash{ // splitmix64 + random offset: no anti-hash tests
	size_t operator()(uint64_t x) const{
		static const uint64_t r = CURRENT_TIMESTAMP;
		x += r + 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
};
template<class K, class V> using hashMap = gp_hash_table<K, V, chash>;
