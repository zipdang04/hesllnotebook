#pragma once
#include "../contest/template.hpp"

// fft: in place, size n = 2^k, a[j] <- sum a[x] w^(jx),
// w = exp(2 pi i / n). conv: real product; convMod: product
// mod a run-time mod (4 FFTs). All O(n log n).
typedef complex<double> Cd;
void fft(vector<Cd> &a){
	int n = sz(a), L = log2(n);
	// the table persists; each root from polar (no drift)
	static vector<Cd> rt(2, 1);
	for(static int k = 2; k < n; k *= 2){
		rt.resize(n);
		FOR(j, 0, k - 1)
			rt[k + j] = polar(1.0L, acosl(-1) * j / k);
	}
	vector<int> rev(n);
	FOR(i, 0, n - 1) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	FOR(i, 0, n - 1) if(i < rev[i]) swap(a[i], a[rev[i]]);
	for(int k = 1; k < n; k *= 2)
		for(int i = 0; i < n; i += 2 * k)
			FOR(j, 0, k - 1){
				Cd z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}
vector<double> conv(const vector<double> &a,
		const vector<double> &b){
	if(a.empty() || b.empty()) return {};
	vector<double> res(sz(a) + sz(b) - 1);
	int n = 1; while(n < sz(res)) n *= 2;
	vector<Cd> in(n), out(n);
	FOR(i, 0, sz(a) - 1) in[i].real(a[i]);
	FOR(i, 0, sz(b) - 1) in[i].imag(b[i]);
	fft(in);
	for(Cd &x: in) x *= x;
	// -i & (n - 1) is (n - i) mod n: the inverse by reversal
	FOR(i, 0, n - 1) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	FOR(i, 0, sz(res) - 1) res[i] = imag(out[i]) / (4 * n);
	return res;
}
vector<ll> convMod(const vector<ll> &a,
		const vector<ll> &b, ll mod){
	if(a.empty() || b.empty()) return {};
	vector<ll> res(sz(a) + sz(b) - 1);
	int n = 1; while(n < sz(res)) n *= 2;
	ll cut = sqrt(mod);
	vector<Cd> fa(n), fb(n), outl(n), outs(n);
	// balanced digits: x = h * cut + l, |h|, |l| <= cut / 2
	auto split = [&](ll x){
		if(x > mod / 2) x -= mod;
		ll h = llround((double)x / cut);
		return Cd(h, x - h * cut);
	};
	FOR(i, 0, sz(a) - 1) fa[i] = split(a[i]);
	FOR(i, 0, sz(b) - 1) fb[i] = split(b[i]);
	fft(fa), fft(fb);
	FOR(i, 0, n - 1){
		int j = -i & (n - 1);
		Cd x = fb[i] / (2.0 * n);
		outl[j] = (fa[i] + conj(fa[j])) * x;
		outs[j] = (fa[i] - conj(fa[j])) * x / Cd(0, 1);
	}
	fft(outl), fft(outs);
	FOR(i, 0, sz(res) - 1){
		ll hi = llround(real(outl[i]));
		ll lo = llround(imag(outs[i]));
		ll mid = llround(imag(outl[i]))
			+ llround(real(outs[i]));
		// digits are signed: x may be negative
		ll x = (hi % mod * cut + mid) % mod * cut + lo;
		res[i] = (x % mod + mod) % mod;
	}
	return res;
}
