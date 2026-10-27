#pragma once
#include "../contest/template.hpp"
template<class T, class F = plus<T>> struct PSeg{ // positions [0, n); F associative, e = identity
	int n; T e; F f; vector<int> L, R; vector<T> v; // node 0 = all-e tree (version 0)
	PSeg(int n, T e = T(), F f = F()): n(n), e(e), f(f), L(1), R(1), v(1, e){}
	int upd(int o, int p, T x, int l = 0, int r = -1){ // set a[p] = x in version o, returns new root
		if (r < 0) r = n;
		int c = L.size(); L.push_back(L[o]); R.push_back(R[o]); v.push_back(x);
		if (r - l > 1){
			int m = (l + r) / 2, t;
			if (p < m) t = upd(L[o], p, x, l, m), L[c] = t;
			else t = upd(R[o], p, x, m, r), R[c] = t;
			v[c] = f(v[L[c]], v[R[c]]);
		}
		return c;
	}
	T qry(int o, int a, int b, int l = 0, int r = -1){ // f over [a, b) in version o
		if (r < 0) r = n;
		if (!o || b <= l || r <= a) return e;
		if (a <= l && r <= b) return v[o];
		int m = (l + r) / 2;
		return f(qry(L[o], a, b, l, m), qry(R[o], a, b, m, r));
	}
};
