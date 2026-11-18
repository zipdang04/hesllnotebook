#pragma once
#include "ntt.hpp"

// Power series over MintN, x^i at index i. inv, log, exp
// and polyDiv in O(n log n), by Newton doubling on NTT.
// polyDiv needs the last term of b nonzero.
vector<MintN> fpsInv(const vector<MintN> &a, int n){
	vector<MintN> g{a[0].inv()};
	for(int m = 1; m < n; m *= 2){
		vector<MintN> f(2 * m), h = g;
		copy_n(a.begin(), min(2 * m, sz(a)), f.begin());
		h.resize(2 * m);
		ntt(f); ntt(h);
		FOR(i, 0, 2 * m - 1)
			f[i] *= h[i];
		ntt(f, true);
		// Low half of a*g is 1, 0, ...; keep only the error.
		fill(f.begin(), f.begin() + m, 0);
		ntt(f);
		FOR(i, 0, 2 * m - 1)
			f[i] *= h[i];
		ntt(f, true);
		g.resize(2 * m);
		FOR(i, m, 2 * m - 1)
			g[i] = -f[i];
	}
	g.resize(n);
	return g;
}
vector<MintN> fpsLog(const vector<MintN> &a, int n){
	vector<MintN> da(n - 1), res(n);
	FOR(i, 0, min(n, sz(a)) - 2)
		da[i] = a[i+1] * (i + 1);
	vector<MintN> c = convNtt(da, fpsInv(a, n));
	FOR(i, 1, n - 1)
		res[i] = c[i-1] / i;
	return res;
}
vector<MintN> fpsExp(const vector<MintN> &a, int n){
	vector<MintN> g{1};
	for(int m = 1; m < n; m *= 2){
		vector<MintN> t = fpsLog(g, 2 * m);
		FOR(i, 0, 2 * m - 1)
			t[i] = (i < sz(a) ? a[i] : MintN(0)) - t[i];
		t[0] += 1;
		g = convNtt(g, t); g.resize(2 * m);
	}
	g.resize(n);
	return g;
}
vector<MintN> polyDiv(const vector<MintN> &a,
		const vector<MintN> &b){
	int k = sz(a) - sz(b) + 1;
	if(k <= 0)
		return {};
	vector<MintN> ra(a.rbegin(), a.rbegin() + k);
	vector<MintN> rb(b.rbegin(), b.rend());
	vector<MintN> q = convNtt(ra, fpsInv(rb, k));
	q.resize(k); reverse(all(q));
	return q;
}
