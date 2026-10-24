#include "../../tex/contents/flow/hungarian.hpp"

mt19937_64 mt;
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }

// dp over column subsets; sg = 1 for min, -1 for max
ll brute(const vector<vector<ll>> &c, int n, int m, int sg){
	vector<ll> dp(1 << m, LLONG_MAX); dp[0] = 0;
	ll best = LLONG_MAX;
	EFOR(int, mask, 0, 1 << m){
		if (dp[mask] == LLONG_MAX) continue;
		int i = __builtin_popcount(mask);
		if (i == n){ best = min(best, dp[mask]); continue; }
		EFOR(int, j, 0, m) if (!(mask >> j & 1))
			dp[mask | 1 << j] = min(dp[mask | 1 << j], dp[mask] + sg * c[i][j]);
	}
	return sg * best;
}

int main(){
	FOR(int, seed, 0, 99999){
		mt.seed(seed);
		int n = rnd(0, 6), m = seed % 10 == 0 ? n : rnd(n, 8);
		ll W = vector<ll>{0, 1, 5, (ll)1e9}[rnd(0, 3)];
		Hungarian h(n, m);
		EFOR(int, i, 0, n) EFOR(int, j, 0, m) h.c[i][j] = rnd(-W, W);
		auto c0 = h.c;
		auto [cost, match] = h.run();
		ll exp = brute(c0, n, m, 1), sum = 0; bool ok = cost == exp && (int)match.size() == n;
		int used = 0;
		if (ok) EFOR(int, i, 0, n){
			int j = match[i];
			if (j < 0 || j >= m || (used >> j & 1)){ ok = false; break; }
			used |= 1 << j; sum += c0[i][j];
		}
		if (ok && sum != cost) ok = false;
		if (ok && seed % 10 == 3){
			auto r2 = h.run();
			if (r2.fi != cost || r2.se != match || h.c != c0) ok = false;
		}
		if (ok && seed % 7 == 0){
			Hungarian g(n, m);
			EFOR(int, i, 0, n) EFOR(int, j, 0, m) g.c[i][j] = -c0[i][j];
			if (-g.run().fi != brute(c0, n, m, -1)) ok = false;
		}
		if (!ok){
			printf("FAIL seed %d n %d m %d got %lld expected %lld\n", seed, n, m, cost, exp);
			EFOR(int, i, 0, n) EFOR(int, j, 0, m) printf("%lld%c", c0[i][j], " \n"[j == m - 1]);
			return 1;
		}
	}
	puts("ok");
}
