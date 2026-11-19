#pragma once
#include "../contest/template.hpp"

// dncDP O(k n log n), recursion depth <= log2 n + 2; knuth
// O(n^2) time and memory; aliens ~42 calls of solve.
// Positions 1..n, C(l, r) on a[l..r]. All minimise.
// Fills cur[l..r]; the opt of mid is in [optl, optr].
template<class Cost> void dncRec(int l, int r, int optl,
		int optr, const vector<ll> &prv, vector<ll> &cur,
		Cost &C){
	if(l > r)
		return;
	int mid = (l + r) / 2, opt = optl;
	cur[mid] = oo;
	FOR(p, optl, min(mid - 1, optr)){
		ll v = prv[p] + C(p + 1, mid);
		// < keeps the smallest opt on ties
		if(v < cur[mid])
			cur[mid] = v, opt = p;
	}
	dncRec(l, mid - 1, optl, opt, prv, cur, C);
	dncRec(mid + 1, r, opt, optr, prv, cur, C);
}
// C: ll(int l, int r) = cost of one block a[l..r].
// Call: dncDP(n, k, [&](int l, int r){ return w(l, r); });
template<class Cost> ll dncDP(int n, int k, Cost C){
	vector<ll> prv(n + 1, oo), cur(n + 1, oo);
	prv[0] = 0;
	// layer j: cur[i], i >= j; its last block is a[p+1..i]
	FOR(j, 1, k){
		dncRec(j, n, j - 1, n - 1, prv, cur, C);
		swap(prv, cur);
	}
	return prv[n];
}
// C(l, r) = cost of merging a[l..r] into one; single = 0.
template<class Cost> ll knuth(int n, Cost C){
	vector<vector<ll>> d(n + 2, vector<ll>(n + 2));
	vector<vector<int>> opt(n + 2, vector<int>(n + 2));
	REV(l, n, 1){
		opt[l][l] = l;
		FOR(r, l + 1, n){
			d[l][r] = oo;
			int hi = min(opt[l+1][r], r - 1);
			FOR(m, opt[l][r-1], hi){
				ll v = d[l][m] + d[m+1][r];
				if(v < d[l][r])
					d[l][r] = v, opt[l][r] = m;
			}
			d[l][r] += C(l, r);
		}
	}
	return d[1][n];
}
// solve: pll(ll lambda) = {min of cost + lambda * cnt,
// smallest cnt reaching it}. Answer for cnt = k exactly.
template<class Solve> ll aliens(int k, Solve solve){
	// every slope f(c + 1) - f(c) lies in [-L, L]
	const ll L = 1e12;
	ll lo = -L, hi = L;
	while(lo < hi){
		ll mid = lo + (hi - lo) / 2;
		if(solve(mid).se <= k)
			hi = mid;
		else
			lo = mid + 1;
	}
	return solve(lo).fi - lo * k;
}
