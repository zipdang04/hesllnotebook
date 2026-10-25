#pragma once
#include "../contest/template.hpp"

// cur[j] = min_{optl <= k <= min(j-1, optr)} prev[k] + C(k, j) for j in [l, r]
template<class F> void dnc(int l, int r, int optl, int optr, const vector<ll> &prev, vector<ll> &cur, F C){
	if (l > r) return;
	int m = (l + r) / 2, best = optl;
	cur[m] = LLONG_MAX;
	FOR(int, k, optl, min(m - 1, optr)){
		ll v = prev[k] + C(k, m);
		if (v < cur[m]) cur[m] = v, best = k;
	}
	dnc(l, m - 1, optl, best, prev, cur, C);
	dnc(m + 1, r, best, optr, prev, cur, C);
}
// dp[l][r] = min_{l <= k < r} dp[l][k] + dp[k+1][r] + C(l, r), dp[i][i] = base(i); returns dp
template<class F, class G> vector<vector<ll>> knuth(int n, F C, G base){
	vector<vector<ll>> dp(n, vector<ll>(n));
	vector<vector<int>> opt(n, vector<int>(n));
	EFOR(int, i, 0, n) dp[i][i] = base(i), opt[i][i] = i;
	REV(int, l, n - 2, 0) EFOR(int, r, l + 1, n){
		dp[l][r] = LLONG_MAX;
		FOR(int, k, opt[l][r - 1], min(r - 1, opt[l + 1][r])){
			ll v = dp[l][k] + dp[k + 1][r] + C(l, r);
			if (v < dp[l][r]) dp[l][r] = v, opt[l][r] = k;
		}
	}
	return dp;
}
// min value using exactly k items, f(k) convex; solve(lam) = {min (value + lam * cnt), min cnt attaining it}
template<class F> ll aliens(ll lo, ll hi, ll k, F solve){
	while (lo < hi){
		ll m = lo + (hi - lo) / 2;
		if (solve(m).se <= k) hi = m; else lo = m + 1;
	}
	return solve(lo).fi - lo * k;
}
