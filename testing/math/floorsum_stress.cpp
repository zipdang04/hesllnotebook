#include "../../tex/contents/math/floorsum.hpp"
#include <cassert>

typedef __int128 i128;
i128 floorDiv(i128 x, i128 y){
	return x / y - (x % y < 0);
}
ll brute(ll n, ll m, ll a, ll b){
	i128 s = 0;
	FOR(i, 0LL, n - 1) s += floorDiv((i128)a * i + b, m);
	return (ll)s;
}
// the note's Needs, in __int128
bool fits(ll n, ll m, ll a, ll b){
	i128 lim = 9e18, N = n, M = m;
	return (N + 1) * M < lim && N * N * ((i128)abs(a) + 2 * M)
		+ 2 * N * ((i128)abs(b) + M) < 2 * lim * M;
}

int main(){
	FOR(n, 0LL, 20) FOR(m, 1LL, 20)
		FOR(a, -20LL, 20) FOR(b, -20LL, 20)
			assert(floorSum(n, m, a, b) == brute(n, m, a, b));
	mt19937_64 rng(1);
	auto rnd = [&](ll lim){ return (ll)(rng() % lim); };
	int cases = 0;
	while(cases < 20000){
		ll n = rnd(2000), m = rnd(rng() % 2 ? 1000 : 1e12) + 1;
		ll lim = rng() % 2 ? 1e9 : 4e15;
		ll a = rnd(2 * lim) - lim, b = rnd(2 * lim) - lim;
		if(rng() % 4 == 0) a = a / m * m;
		if(rng() % 4 == 0) b = -m + rnd(3);
		if(!fits(n, m, a, b)) continue;
		cases++;
		ll s = floorSum(n, m, a, b);
		assert(s == brute(n, m, a, b));
		i128 lin = 0, mods = 0;  // the note's Result identity
		FOR(i, 0LL, n - 1){
			lin += (i128)a * i + b;
			mods += (((i128)a * i + b) % m + m) % m;
		}
		assert(mods == lin - (i128)m * s);
	}
	// near the bounds of the Needs, n large
	vector<array<ll, 4>> edge = {
		{1, 1, 0, (ll)2e18}, {1, 1, 0, (ll)-2e18},
		{2, (ll)2.9e18, (ll)-1e9, (ll)-1},
		{3000000, 1, -400000, 7},
		{1000000, 999999, -999998, -999998},
		{2000000, (ll)1e12, (ll)-1e12 + 1, (ll)1e12 - 1},
		{5000000, 3, -100000, -1},
		{30000000, 1000000007, 5, -1},
		// m > (n + 1)^2 with |a| or |b| near 2^63 (review b3miss)
		{0, (ll)5e18, (ll)-9e18, 0}, {1, (ll)4.4e18, (ll)-9e18, 0},
		{1, (ll)4.4e18, 0, (ll)-9e18}, {0, (ll)5e18, (ll)4.5e18, 0}};
	for(auto [n, m, a, b]: edge){
		assert(fits(n, m, a, b));
		assert(floorSum(n, m, a, b) == brute(n, m, a, b));
	}
}
