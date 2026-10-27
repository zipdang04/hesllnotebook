#pragma once
#include "../contest/template.hpp"

// C(l, r) = cost of a[l..r] (1-based, inclusive). Impossible = LLONG_MAX.
// dp_i[j] = min_{k<j} dp_{i-1}[k] + C(k+1, j), dp_0[0] = 0; returns dp_K[0..n]
template<class F> vector<ll> dncDP(int n, int K, F C){
	vector<ll> prev(n + 1, LLONG_MAX), cur(n + 1); prev[0] = 0;
	auto rec = [&](auto &self, int l, int r, int optl, int optr)->void{
		if (l > r) return;
		int m = (l + r) / 2, best = optl;
		cur[m] = LLONG_MAX;
		FOR(int, k, optl, min(m - 1, optr)) if (prev[k] < LLONG_MAX){
			ll v = prev[k] + C(k + 1, m);
			if (v < cur[m]) cur[m] = v, best = k;
		}
		self(self, l, m - 1, optl, best), self(self, m + 1, r, best, optr);
	};
	EFOR(int, i, 0, K) cur[0] = LLONG_MAX, rec(rec, 1, n, 0, n - 1), swap(prev, cur);
	return prev;
}
// dp[l][r] = min_{l<=k<r} dp[l][k] + dp[k+1][r] + C(l, r), dp[i][i] = 0; dp[1][n] = answer
template<class F> vector<vector<ll>> knuth(int n, F C){
	vector<vector<ll>> dp(n + 1, vector<ll>(n + 1));
	vector<vector<int>> opt(n + 1, vector<int>(n + 1));
	FOR(int, i, 1, n) opt[i][i] = i;
	REV(int, l, n - 1, 1) FOR(int, r, l + 1, n){
		dp[l][r] = LLONG_MAX;
		FOR(int, k, opt[l][r - 1], min(r - 1, opt[l + 1][r])){
			ll v = dp[l][k] + dp[k + 1][r] + C(l, r);
			if (v < dp[l][r]) dp[l][r] = v, opt[l][r] = k;
		}
	}
	return dp;
}
// f(k) = min value with exactly k items, convex in k. solve(lam) = {min (value + lam*count),
// SMALLEST count attaining it}. Needs lo <= f(k-1) - f(k), hi >= f(k) - f(k+1).
template<class F> ll aliens(ll k, F solve, ll lo = -1e12, ll hi = 1e12){
	while (lo < hi){
		ll m = lo + (hi - lo) / 2;
		if (solve(m).se <= k) hi = m; else lo = m + 1;
	}
	return solve(lo).fi - lo * k;
}
