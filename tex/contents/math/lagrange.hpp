#pragma once
#include "../contest/template.hpp"
#include "modint.hpp"
// y[i] = f(i), i = 0..n-1, deg f < n; returns f(X mod p). O(n), needs n <= p
modint lagrangeAt(const vector<modint> &y, ll X){
	int n = y.size();
	if (!n) return 0;
	vector<modint> pre(n + 1, 1), suf(n + 1, 1), f(n, 1); // f[i] = 1/i! after the loop
	FOR(int, i, 0, n - 1) pre[i + 1] = pre[i] * (X - i);
	REV(int, i, n - 1, 0) suf[i] = suf[i + 1] * (X - i);
	FOR(int, i, 1, n - 1) f[i] = f[i - 1] * i;
	f[n - 1] = f[n - 1].inv();
	REV(int, i, n - 1, 1) f[i - 1] = f[i] * i;
	modint r = 0;
	FOR(int, i, 0, n - 1){
		modint t = y[i] * pre[i] * suf[i + 1] * f[i] * f[n - 1 - i];
		r += (n - 1 - i) & 1 ? -t : t;
	}
	return r;
}
// distinct x[i], y[i] = f(x[i]); returns coefficients c[0..n-1] of f, c[j] of X^j. O(n^2)
vector<modint> interpolate(const vector<modint> &x, const vector<modint> &y){
	int n = x.size();
	vector<modint> P(n + 1), c(n); // P = prod (X - x[i])
	P[0] = 1;
	FOR(int, i, 0, n - 1) REV(int, j, i + 1, 0)
		P[j] = (j ? P[j - 1] : 0) - P[j] * x[i];
	FOR(int, i, 0, n - 1){
		vector<modint> q(n); // q = P / (X - x[i]) by synthetic division
		modint b = 0, v = 0;
		REV(int, j, n, 1){ b = P[j] + b * x[i]; q[j - 1] = b; v = v * x[i] + b; }
		modint w = y[i] / v; // v = q(x[i])
		FOR(int, j, 0, n - 1) c[j] += q[j] * w;
	}
	return c;
}
