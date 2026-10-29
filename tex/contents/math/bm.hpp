#pragma once
#include "../contest/template.hpp"
#include "fps.hpp"

// mod 998244353; 0-indexed: s[i] = sum_j c[j] s[i-1-j]
FPS::poly berlekampMassey(const FPS::poly &s){ // shortest c
	int n = s.size(), L = 0, m = 0;
	FPS::poly C(n+1), B(n+1), T; C[0] = B[0] = 1;
	modint b = 1;
	FOR(int, i, 0, n - 1){ ++m;
		modint d = s[i];
		FOR(int, j, 1, L) d += C[j] * s[i-j];
		if (d == 0) continue;
		T = C; modint coef = d / b;
		FOR(int, j, m, n) C[j] -= coef * B[j-m];
		if (2*L > i) continue;
		L = i + 1 - L; B = T; b = d; m = 0;
	}
	C.resize(L+1); C.erase(C.begin());
	for (auto &x: C) x = -x;
	return C;
}
modint bostanMori(FPS::poly P, FPS::poly Q, ll k){ // [x^k] P/Q, Q[0] != 0
	FFT::buildRoot(); int q = Q.size(); modint h = (modint::MOD + 1) / 2;
	for (; k; k /= 2){ // P/Q = P(x)Q(-x) / Q(x)Q(-x), keep exponents of parity k
		int n = 2, m = (P.size() + q) / 2;
		while (n < max((int)P.size(), q) + q) n *= 2;
		P.resize(n); Q.resize(n); FFT::transform(P, 0); FFT::transform(Q, 0);
		FOR(int, i, 0, n/2 - 1){ // value at -w^i is at index i + n/2; _root[..] = w^-i
			modint a = P[i] * Q[i+n/2], b = P[i+n/2] * Q[i];
			P[i] = k & 1 ? (a - b) * h * FFT::_root[(n-i) * (FFT::MAX_LEN/n)] : (a + b) * h;
			Q[i] *= Q[i+n/2];
		}
		P.resize(n/2); Q.resize(n/2); FFT::transform(P, 1); FFT::transform(Q, 1);
		P.resize(m); Q.resize(q);
	}
	return P.empty() ? 0 : P[0] / Q[0];
}
modint kthTerm(const FPS::poly &a, const FPS::poly &c, ll k){ // a: first |c| terms
	int d = c.size(); FPS::poly Q(d+1); Q[0] = 1;
	FOR(int, j, 0, d - 1) Q[j+1] = -c[j];
	FPS::poly P = FPS::mul(a, Q); P.resize(d);
	return bostanMori(P, Q, k);
}
