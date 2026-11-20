#include "../../tex/contents/math/xorbasis.hpp"
#include <cassert>

void check(const vector<ll> &a){
	XorBasis b;
	vector<ll> sums{0};
	assert(b.maxXor() == 0 && b.kth(0) == 0);
	assert(b.representable(0) && !b.representable(1));
	for(ll x: a){
		bool fresh = !binary_search(all(sums), x);
		assert(b.insert(x) == fresh);
		int n = sz(sums);
		FOR(i, 0, n - 1) sums.push_back(sums[i] ^ x);
		sort(all(sums));
		sums.erase(unique(all(sums)), sums.end());
		assert((1LL << sz(b.basis)) == sz(sums));
		assert(b.maxXor() == sums.back());
		FOR(k, 0, sz(sums) - 1){
			assert(b.kth(k) == sums[k]);
			assert(b.representable(sums[k]));
			assert(b.reduce(sums[k]) == 0);
		}
		FOR(y, 0LL, 255){
			bool has = binary_search(all(sums), y);
			assert(b.representable(y) == has);
			ll best = LLONG_MAX;
			for(ll s: sums) best = min(best, y ^ s);
			assert(b.reduce(y) == best);
		}
	}
	b = XorBasis(); assert(b.basis.empty());
}

int main(){
	check({}); check({0, 0}); check({3, 5, 6, 3});
	check({1LL << 59, (1LL << 60) - 1, 1, 1LL << 59});
	mt19937_64 rng(1);
	FOR(test, 1, 2'000){
		vector<ll> a(rng() % 9);
		for(ll &x: a) x = rng() % 256;
		check(a);
		for(ll &x: a) x = rng() & ((1LL << 60) - 1);
		check(a);
	}
	XorBasis full;
	FOR(i, 0, 59) assert(full.insert(1LL << i));
	assert(!full.insert((1LL << 60) - 1));
	assert(full.maxXor() == (1LL << 60) - 1);
	FOR(k, 0LL, 1'000) assert(full.kth(k) == k);
	assert(full.kth((1LL << 60) - 1) == full.maxXor());
}
