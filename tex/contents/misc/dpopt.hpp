#pragma once
#include "../contest/template.hpp"
template<class F> vector<ll> dncDP(int n, int K, F C){
	vector<ll> prev(n + 1, LLONG_MAX), cur(n + 1);
	prev[0] = 0;
	auto rec = [&](auto &self, int l, int r, int a, int b){
		if (l > r) return;
		int m = (l + r) / 2, best = a;
		cur[m] = LLONG_MAX;
		FOR(int, k, a, min(m - 1, b)){
			if (prev[k] == LLONG_MAX) continue;
			ll v = prev[k] + C(k + 1, m);
			if (v < cur[m]){ cur[m] = v; best = k; }
		}
		self(self, l, m - 1, a, best);
		self(self, m + 1, r, best, b);
	};
	FOR(int, i, 1, K){
		cur[0] = LLONG_MAX; rec(rec, 1, n, 0, n - 1);
		swap(prev, cur);
	}
	return prev;
}
template<class F> vector<vector<ll>> knuth(int n, F C){
	vector<vector<ll>> dp(n + 1, vector<ll>(n + 1));
	vector<int> o(n + 1);
	REV(int, l, n, 1){
		o[l] = l;
		FOR(int, r, l + 1, n){
			int lo = o[r - 1], hi = min(r - 1, o[r]);
			dp[l][r] = LLONG_MAX;
			FOR(int, k, lo, hi){
				ll v = dp[l][k] + dp[k + 1][r] + C(l, r);
				if (v < dp[l][r]){ dp[l][r] = v; o[r] = k; }
			}
		}
	}
	return dp;
}
template<class F>
ll aliens(ll k, F solve, ll lo = -1e12, ll hi = 1e12){
	while (lo < hi){
		ll m = lo + (hi - lo) / 2;
		if (solve(m).se <= k) hi = m; else lo = m + 1;
	}
	return solve(lo).fi - lo * k;
}
