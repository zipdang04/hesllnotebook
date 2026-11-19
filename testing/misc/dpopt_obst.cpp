#include "../../tex/contents/misc/dpopt.hpp"
#include <cassert>

int main(){
	vector<ll> s = {0, 3, 4, 8, 9, 14};
	auto C = [&](int l, int r){
		ll v = s[r] - s[l-1];
		return v * v;
	};
	vector<ll> want = {0, 196, 100, 66, 58, 52};
	auto solve = [&](ll lambda){
		vector<pll> g(6, {oo, 0});
		g[0] = {0, 0};
		FOR(i, 1, 5)
			FOR(p, 0, i - 1){
				pll v = {g[p].fi + C(p + 1, i) + lambda,
					g[p].se + 1};
				minimize(g[i], v);
			}
		return g[5];
	};
	FOR(k, 1, 5){
		assert(dncDP(5, k, C) == want[k]);
		assert(aliens(k, solve) == want[k]);
	}
	auto sum = [&](int l, int r){ return s[r] - s[l-1]; };
	assert(knuth(5, sum) == 32);
	assert(knuth(1, sum) == 0);
	assert(dncDP(1, 1, C) == 9);
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n;
	if(!(cin >> n))
		return 0;
	auto read = [&](){
		string t; cin >> t;
		ll v = 0;
		for(char c: t) if(c != '.')
			v = 10 * v + c - '0';
		return v;
	};
	vector<ll> sp(n + 2), sq(n + 2);
	FOR(i, 1, n)
		sp[i] = sp[i-1] + read();
	FOR(i, 1, n + 1)
		sq[i] = sq[i-1] + read();
	ll v = knuth(n + 1, [&](int l, int r){
		return sq[r] - sq[l-1] + sp[r-1] - sp[l-1];
	});
	v += sq[n+1];
	cout << v / 10000 << '.' << setfill('0') << setw(4)
		<< v % 10000 << '\n';
}
