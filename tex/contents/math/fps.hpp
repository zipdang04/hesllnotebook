#pragma once
#include "../contest/template.hpp"
#include "ntt.hpp"

// power series mod 998244353; n = number of wanted terms
namespace FPS{
	typedef vector<modint> poly;
	poly mul(poly a, poly b){ // exact product, size |a|+|b|-1
		if (a.empty() || b.empty()) return {};
		int n = a.size() + b.size() - 1; FFT::buildRoot();
		a = FFT::multiply(a, b); a.resize(n); return a;
	}
	poly inv(const poly &a, int n){ // a[0] != 0
		poly r{a[0].inv()};
		for (int k = 1; k < n; k *= 2){ // r = r (2 - a r) mod x^2k
			poly t = mul(r, poly(a.begin(), a.begin() + min((int)a.size(), 2*k)));
			t.resize(2*k); for (auto &x: t) x = -x; t[0] += 2;
			r = mul(r, t); r.resize(2*k);
		}
		r.resize(n); return r;
	}
	poly log(const poly &a, int n){ // a[0] == 1; log a = int a'/a
		poly d;
		EFOR(int, i, 1, min((int)a.size(), n)) d.push_back(a[i] * i);
		d = mul(d, inv(a, n)); d.resize(n);
		REV(int, i, n-1, 1) d[i] = d[i-1] / i;
		d[0] = 0; return d;
	}
	poly exp(const poly &a, int n){ // a[0] == 0
		poly r{1};
		for (int k = 1; k < n; k *= 2){ // r = r (1 + a - log r) mod x^2k
			poly t = log(r, 2*k);
			EFOR(int, i, 0, 2*k) t[i] = (i < (int)a.size() ? a[i] : 0) - t[i];
			t[0] += 1; r = mul(r, t); r.resize(2*k);
		}
		r.resize(n); return r;
	}
	pair<poly, poly> divMod(poly a, poly b){ // a = b q + r, deg r < deg b; b.back() != 0
		int n = a.size(), m = b.size();
		if (n < m) return {{}, a};
		reverse(a.begin(), a.end()); reverse(b.begin(), b.end());
		poly q = mul(a, inv(b, n-m+1)); q.resize(n-m+1);
		reverse(q.begin(), q.end());
		reverse(a.begin(), a.end()); reverse(b.begin(), b.end());
		poly r = mul(b, q); r.resize(m-1);
		EFOR(int, i, 0, m-1) r[i] = a[i] - r[i];
		return {q, r};
	}
}
