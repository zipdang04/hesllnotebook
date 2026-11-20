#pragma once
#include "modint.hpp"

// Lagrange interpolation over Mint<MOD>, MOD prime.
// interpolate: distinct x, c[k] of t^k, deg < n; O(n^2).
// interpolateAt: y[i] = f(i), i = 0..n-1; f(t) in O(n).
template<int MOD>
vector<Mint<MOD>> interpolate(const vector<Mint<MOD>> &x,
		const vector<Mint<MOD>> &y){
	int n = sz(x);
	vector<Mint<MOD>> p(n + 1), q(n), c(n); p[0] = 1;
	FOR(i, 0, n - 1){
		REV(k, i + 1, 1) p[k] = p[k-1] - x[i] * p[k];
		p[0] = -x[i] * p[0];
	}
	FOR(i, 0, n - 1){
		Mint<MOD> w = 1;
		FOR(j, 0, n - 1) if(i != j) w *= x[i] - x[j];
		w = y[i] / w;
		q[n-1] = 1;
		REV(k, n - 1, 1) q[k-1] = p[k] + x[i] * q[k];
		FOR(k, 0, n - 1) c[k] += w * q[k];
	}
	return c;
}
template<int MOD>
Mint<MOD> interpolateAt(const vector<Mint<MOD>> &y, ll t){
	int n = sz(y);
	Mint<MOD> z = t, res = 0;
	vector<Mint<MOD>> pre(n + 1, 1), suf(n + 1, 1);
	vector<Mint<MOD>> ifac(n, 1);
	FOR(i, 0, n - 1) pre[i+1] = pre[i] * (z - i);
	REV(i, n - 1, 0) suf[i] = suf[i+1] * (z - i);
	FOR(i, 1, n - 1) ifac[n-1] *= i;
	ifac[n-1] = ifac[n-1].inv();
	REV(i, n - 1, 1) ifac[i-1] = ifac[i] * i;
	FOR(i, 0, n - 1){
		Mint<MOD> term = y[i] * pre[i] * suf[i+1]
			* ifac[i] * ifac[n-1-i];
		// Sign of the product of (i - j) for j > i.
		res += (n - 1 - i) % 2 ? -term : term;
	}
	return res;
}
