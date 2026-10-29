#pragma once
#include "modint.hpp"

// f(a[sub], a[super]) over every submask pair; sup=0: a[S]=f-fold over subsets of S, sup=1: over supersets
template<class T, class F> void sos(vector<T> &a, F f, bool sup){
	int n = a.size();
	for (int h = 1; h < n; h *= 2) for (int i = 0; i < n; i += 2 * h) FOR(int, j, i, i + h - 1){
		T &x = a[j], &y = a[j + h];
		if (sup) x = f(x, y); else y = f(y, x);
	}
}
// op: '^', '|' or '&'; |a| = 2^k; inv=1 undoes it (xor needs T with /, e.g. modint)
template<class T> void fwht(vector<T> &a, char op, bool inv){
	int n = a.size();
	if (op == '^'){
		for (int h = 1; h < n; h *= 2) for (int i = 0; i < n; i += 2 * h) FOR(int, j, i, i + h - 1){
			T u = a[j], v = a[j + h]; a[j] = u + v; a[j + h] = u - v;
		}
		if (inv) for (T &v: a) v = v / n;
	} else sos(a, [&](T p, T q){ return inv ? p - q : p + q; }, op == '&');
}
template<class T> vector<T> convolve(vector<T> a, vector<T> b, char op){
	fwht(a, op, 0); fwht(b, op, 0);
	FOR(int, i, 0, (int)a.size() - 1) a[i] = a[i] * b[i];
	return fwht(a, op, 1), a;
}
