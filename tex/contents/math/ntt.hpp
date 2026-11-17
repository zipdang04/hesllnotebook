#pragma once
#include "modint.hpp"

// ntt: in place, size 2^k <= 2^23 over MintN (root 3);
// ntt(a, true) is the inverse. convNtt: polynomial product,
// O(n log n); naive when one side has <= 32 terms.
void ntt(vector<MintN> &a, bool inv = false){
	int n = sz(a), L = log2(n);
	// the table persists across calls
	static vector<MintN> rt(2, 1);
	for(static int k = 2, s = 2; k < n; k *= 2, s++){
		rt.resize(n);
		// MintN(-1).v is MOD - 1
		MintN z[] = {1, MintN(3).pow(MintN(-1).v >> s)};
		FOR(i, k, 2 * k - 1) rt[i] = rt[i / 2] * z[i & 1];
	}
	vector<int> rev(n);
	FOR(i, 0, n - 1) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	FOR(i, 0, n - 1) if(i < rev[i]) swap(a[i], a[rev[i]]);
	for(int k = 1; k < n; k *= 2)
		for(int i = 0; i < n; i += 2 * k)
			FOR(j, 0, k - 1){
				MintN z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
	if(!inv) return;
	reverse(a.begin() + 1, a.end());
	MintN invN = MintN(n).inv();
	for(MintN &x: a) x *= invN;
}
vector<MintN> convNtt(vector<MintN> a, vector<MintN> b){
	if(a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, n = 1;
	const int NAIVE = 32;
	if(min(sz(a), sz(b)) <= NAIVE){
		vector<MintN> c(s);
		FOR(i, 0, sz(a) - 1)
			FOR(j, 0, sz(b) - 1)
				c[i + j] += a[i] * b[j];
		return c;
	}
	while(n < s) n <<= 1;
	a.resize(n), b.resize(n);
	ntt(a), ntt(b);
	FOR(i, 0, n - 1) a[i] *= b[i];
	ntt(a, true);
	a.resize(s);
	return a;
}
