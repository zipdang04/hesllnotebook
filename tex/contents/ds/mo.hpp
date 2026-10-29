#pragma once
#include "../contest/template.hpp"

// Q[i] = [l, r], 1-indexed inclusive, 1 <= l <= r (r < 2^20 for the Hilbert key); window starts empty
// add(i, e) / del(i, e): put / take a[i] in / out of the window at its left (e = 0) or right (e = 1) end
// calc(): answer of the current window; returns res[i] = calc() on window Q[i]
template<class A, class D, class C> auto mo(const vector<pair<int,int>>& Q, A add, D del, C calc){
	auto hil = [](int x, int y){ // position of (x, y) on the Hilbert curve
		ll d = 0;
		for (int s = 1 << 19; s; s >>= 1){
			bool rx = x & s, ry = y & s;
			d += (ll)s * s * ((3 * rx) ^ ry);
			if (!ry){ if (rx) x ^= s - 1, y ^= s - 1; swap(x, y); }
		}
		return d;
	};
	int q = Q.size(), L = 1, R = 0;
	vector<int> s(q); vector<ll> h(q); vector<decltype(calc())> res(q);
	FOR(int, i, 0, q - 1) s[i] = i, h[i] = hil(Q[i].fi, Q[i].se);
	sort(s.begin(), s.end(), [&](int a, int b){ return h[a] < h[b]; });
	for (int i: s){
		auto [l, r] = Q[i];
		while (L > l) add(--L, 0);
		while (R < r) add(++R, 1);
		while (L < l) del(L++, 0);
		while (R > r) del(R--, 1);
		res[i] = calc();
	}
	return res;
}
