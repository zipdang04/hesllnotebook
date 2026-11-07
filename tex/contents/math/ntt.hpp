#pragma once
#include "modint.hpp"
void ntt(vector<modint> &a){
	int n = a.size(), L = log2(n);
	static vector<modint> rt(2, 1);
	for (static int k = 2; k < n; k *= 2){
		rt.resize(n);
		modint z = modint(3) ^ (modint::MOD - 1) / (2 * k);
		FOR(int, j, k, 2 * k - 1)
			rt[j] = j & 1 ? rt[j / 2] * z : rt[j / 2];
	}
	vector<int> rev(n);
	FOR(int, i, 0, n - 1){
		rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	}
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k)
			FOR(int, j, 0, k - 1){
				modint z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}
void intt(vector<modint> &a){
	reverse(a.begin() + 1, a.end()); ntt(a);
	// n | MOD - 1, and n (MOD - (MOD - 1) / n) = 1 mod MOD
	modint inv = modint::MOD - (modint::MOD - 1) / a.size();
	for (modint &x: a) x *= inv;
}
vector<modint> conv(vector<modint> a, vector<modint> b){
	if (a.empty() || b.empty()) return {};
	int s = a.size() + b.size() - 1, n = 1;
	while (n < s) n *= 2;
	a.resize(n); b.resize(n);
	ntt(a); ntt(b);
	FOR(int, i, 0, n - 1) a[i] *= b[i];
	intt(a); a.resize(s); return a;
}
