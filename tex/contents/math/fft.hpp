#pragma once
#include "../contest/template.hpp"
typedef complex<double> C;
// in place, |a| = 2^k: a[j] <- sum_x a[x] w^(jx), w = exp(2 pi i / |a|)
void fft(vector<C> &a){
	int n = a.size(), L = log2(n);
	static vector<C> rt(2, 1); // rt[k + j] = w_2k^j, each from cos/sin (no drift)
	for (static int k = 2; k < n; k *= 2){
		rt.resize(n);
		EFOR(int, j, 0, k) rt[k + j] = polar(1.0L, acosl(-1) * j / k);
	}
	vector<int> rev(n);
	EFOR(int, i, 0, n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	EFOR(int, i, 0, n) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2) for (int i = 0; i < n; i += 2 * k) EFOR(int, j, 0, k){
		C z = rt[j + k] * a[i + j + k];
		a[i + j + k] = a[i + j] - z; a[i + j] += z;
	}
}
// c[k] = sum a[i] b[k-i]; round for integers
vector<double> conv(const vector<double> &a, const vector<double> &b){
	if (a.empty() || b.empty()) return {};
	vector<double> res(a.size() + b.size() - 1);
	int n = 2 << log2(res.size());
	vector<C> in(n), out(n);
	EFOR(int, i, 0, a.size()) in[i].real(a[i]);
	EFOR(int, i, 0, b.size()) in[i].imag(b[i]);
	fft(in);
	for (C &x: in) x *= x;
	EFOR(int, i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	EFOR(int, i, 0, res.size()) res[i] = imag(out[i]) / (4 * n);
	return res;
}
// a, b in [0, M), result mod M
template<int M> vector<ll> convMod(const vector<ll> &a, const vector<ll> &b){
	if (a.empty() || b.empty()) return {};
	vector<ll> res(a.size() + b.size() - 1);
	int n = 2 << log2(res.size()), cut = sqrt(M);
	vector<C> L(n), R(n), outl(n), outs(n);
	auto split = [&](ll x){ // balanced digits: x = h*cut + l, |h|, |l| <= cut/2
		if (x > M / 2) x -= M;
		ll h = llround((double)x / cut); return C(h, x - h * cut);
	};
	EFOR(int, i, 0, a.size()) L[i] = split(a[i]);
	EFOR(int, i, 0, b.size()) R[i] = split(b[i]);
	fft(L); fft(R);
	EFOR(int, i, 0, n){
		int j = -i & (n - 1);
		outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
		outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / C(0, 1);
	}
	fft(outl); fft(outs);
	EFOR(int, i, 0, res.size()){
		ll hi = llround(real(outl[i])), lo = llround(imag(outs[i]));
		ll mid = llround(imag(outl[i])) + llround(real(outs[i]));
		res[i] = (((hi % M * cut + mid) % M * cut + lo) % M + M) % M;
	}
	return res;
}
