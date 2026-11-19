#pragma once
#include "ntt.hpp"

// berlekampMassey O(n^2); kthTerm by Bostan-Mori,
// O(d log d log k), 2d + 1 <= 2^23. s, c and k are 0-based
// (s[0] first).
vector<MintN> berlekampMassey(const vector<MintN> &s){
	int n = sz(s), L = 0, m = 0;
	// s may be empty; C[0] and B[0] must exist.
	vector<MintN> C(n + 1), B(n + 1), T;
	C[0] = B[0] = 1;
	MintN b = 1;
	FOR(i, 0, n - 1){
		++m;
		MintN d = s[i];
		FOR(j, 1, L) d += C[j] * s[i - j];
		if(d == 0) continue;
		T = C; MintN coef = d / b;
		FOR(j, m, n - 1) C[j] -= coef * B[j - m];
		if(2 * L > i) continue;
		L = i + 1 - L, B = T, b = d, m = 0;
	}
	C.resize(L + 1); C.erase(C.begin());
	for(MintN &x: C) x = -x;
	return C;
}
MintN kthTerm(const vector<MintN> &s,
		const vector<MintN> &c, ll k){
	int d = sz(c);
	if(d == 0) return 0;
	if(k < d) return s[k];
	vector<MintN> Q(d + 1);
	Q[0] = 1;
	FOR(i, 0, d - 1) Q[i + 1] = -c[i];
	vector<MintN> P(s.begin(), s.begin() + d);
	P = convNtt(P, Q); P.resize(d);
	for(; k; k >>= 1){
		// R is Q(-x).
		vector<MintN> R = Q;
		for(int i = 1; i <= d; i += 2) R[i] = -R[i];
		vector<MintN> U = convNtt(P, R);
		vector<MintN> V = convNtt(Q, R);
		FOR(i, 0, d - 1) P[i] = U[2 * i + (k & 1)];
		FOR(i, 0, d) Q[i] = V[2 * i];
	}
	// Q[0] stays 1.
	return P[0];
}
