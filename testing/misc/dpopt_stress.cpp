#include "../../tex/contents/misc/dpopt.hpp"
#include <cassert>

template<class Cost> void checkPartitions(int n, Cost C){
	vector<vector<ll>> f(n + 1, vector<ll>(n + 1, oo));
	f[0][0] = 0;
	FOR(k, 1, n){
		FOR(i, k, n){
			FOR(p, k - 1, i - 1){
				if(f[k-1][p] == oo)
					continue;
				minimize(f[k][i], f[k-1][p] + C(p+1, i));
			}
		}
		vector<ll> cur(n + 1, -oo);
		dncRec(k, n, k - 1, n - 1, f[k-1], cur, C);
		FOR(i, 0, n)
			assert(cur[i] == (i < k ? -oo : f[k][i]));
		assert(dncDP(n, k, C) == f[k][n]);
	}
	auto solve = [&](ll lambda){
		vector<pll> d(n + 1, {oo, 0});
		d[0] = {0, 0};
		FOR(i, 1, n){
			FOR(p, 0, i - 1){
				pll v = {d[p].fi + C(p+1, i) + lambda,
					d[p].se + 1};
				minimize(d[i], v);
			}
		}
		return d[n];
	};
	FOR(k, 1, n)
		assert(aliens(k, solve) == f[k][n]);
}

template<class Cost> void checkKnuth(int n, Cost C){
	vector<vector<ll>> d(n + 1, vector<ll>(n + 1));
	FOR(len, 2, n){
		FOR(l, 1, n - len + 1){
			int r = l + len - 1;
			d[l][r] = oo;
			FOR(m, l, r - 1)
				minimize(d[l][r],
					d[l][m] + d[m+1][r] + C(l, r));
		}
	}
	assert(knuth(n, C) == d[1][n]);
}

int main(){
	mt19937_64 rng(712367821);
	FOR(trial, 0, 2999){
		int n = trial == 0 ? 1 : (int)(rng() % 30) + 1;
		vector<ll> s(n + 1), g(n + 1), h(n + 1);
		FOR(i, 1, n){
			// All-zero cases exercise tied optima.
			ll a = trial < 6 ? 0 : rng() % 20;
			s[i] = s[i-1] + a;
			g[i] = trial < 6 ? 0 : (ll)(rng() % 41) - 20;
			h[i] = trial < 6 ? 0 : (ll)(rng() % 41) - 20;
		}
		vector<vector<ll>> w(n + 1, vector<ll>(n + 1));
		FOR(l, 1, n) FOR(r, l, n){
			ll x = s[r] - s[l-1], len = r - l;
			ll v = x * x;
			if(trial % 3 == 1)
				v += len * len;
			if(trial % 3 == 2)
				v *= x;
			if(trial % 6 >= 3)
				v += g[l] + h[r];
			w[l][r] = v;
		}
		auto C = [&](int l, int r){
			assert(1 <= l && l <= r && r <= n);
			return w[l][r];
		};
		checkPartitions(n, C);
		FOR(power, 1, 2){
			auto mergeCost = [&](int l, int r){
				assert(1 <= l && l <= r && r <= n);
				ll x = s[r] - s[l-1];
				return power == 1 ? x : x * x;
			};
			checkKnuth(n, mergeCost);
		}
	}
	// Convex tables reach both ends of the lambda range.
	const ll L = 1000000000000LL;
	vector<ll> slopes = {-L, -L + 1, -1, 0, 1, L - 1, L};
	FOR(trial, 0, 99){
		int n = (int)(rng() % 30) + 1;
		vector<ll> delta(n - 1), f(n + 1);
		for(ll &v: delta)
			v = slopes[rng() % sz(slopes)];
		sort(all(delta));
		FOR(i, 2, n)
			f[i] = f[i-1] + delta[i-2];
		auto solve = [&](ll lambda){
			pll best = {oo, 0};
			FOR(c, 1, n){
				pll v = {f[c] + lambda * c, c};
				minimize(best, v);
			}
			return best;
		};
		FOR(k, 1, n)
			assert(aliens(k, solve) == f[k]);
	}
}
