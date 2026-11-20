#pragma once
#include "modint.hpp"

// convBit: c[i op j] += a[i] * b[j], op '^', '&' or '|'.
// fwht: in place, size 2^k, O(n log n); forward '|' sums
// over subsets, forward '&' over supersets.
template<int MOD>
void fwht(vector<Mint<MOD>> &a, char op, bool inv = false){
	int n = sz(a);
	for(int k = 1; k < n; k *= 2)
		for(int i = 0; i < n; i += 2 * k)
			FOR(j, i, i + k - 1){
				Mint<MOD> u = a[j], v = a[j + k];
				if(op == '^') a[j] = u + v, a[j + k] = u - v;
				if(op == '&') a[j] = inv ? u - v : u + v;
				if(op == '|') a[j + k] = inv ? v - u : u + v;
			}
	if(op != '^' || !inv) return;
	Mint<MOD> invN = Mint<MOD>(n).inv();
	for(Mint<MOD> &x: a) x *= invN;
}
template<int MOD>
vector<Mint<MOD>> convBit(vector<Mint<MOD>> a,
		vector<Mint<MOD>> b, char op){
	int n = 1; while(n < max(sz(a), sz(b))) n *= 2;
	a.resize(n), b.resize(n);
	fwht(a, op), fwht(b, op);
	FOR(i, 0, n - 1) a[i] *= b[i];
	fwht(a, op, true);
	return a;
}
