#include "../../tex/contents/misc/dpopt.hpp"

mt19937_64 mt;
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
const ll INF = LLONG_MAX;
void fail(const char *what, int seed){ printf("FAIL %s seed %d\n", what, seed); exit(1); }

int main(){
	for (int seed = 0; seed < 200000; seed++){
		mt.seed(seed);
		int n = seed % 7 == 0 ? rnd(1, 3) : rnd(1, 12);
		// QI + monotone costs of a[l..r] (1-based): fam 0 (sum)^2 with a >= 0, fam 1 sum of w over
		// sub-intervals, fam 2 plain sum (QI with equality); many zeros give ties
		int fam = rnd(0, 2);
		ll W = vector<ll>{0, 1, 10, (ll)1e6}[rnd(0, 3)];
		vector<vector<ll>> w(n + 2, vector<ll>(n + 2, 0)), I(n + 2, vector<ll>(n + 2, 0));
		vector<ll> S(n + 1, 0);
		FOR(int, i, 1, n) S[i] = S[i - 1] + (rnd(0, 2) ? rnd(0, W) : 0);
		FOR(int, l, 1, n) FOR(int, r, l, n) if (rnd(0, 3) == 0) w[l][r] = rnd(0, W);
		REV(int, l, n, 1) FOR(int, r, l, n)
			I[l][r] = w[l][r] + (r > l ? I[l + 1][r] + I[l][r - 1] - (l + 1 <= r - 1 ? I[l + 1][r - 1] : 0) : 0);
		auto C = [&](int l, int r)->ll{
			ll x = S[r] - S[l - 1];
			return fam == 0 ? x * x : fam == 1 ? I[l][r] : x;
		};

		// brute: f[i][j] = min cost of splitting a[1..j] into i groups (INF if impossible), i up to n + 2
		vector<vector<ll>> f(n + 3, vector<ll>(n + 1, INF));
		f[0][0] = 0;
		FOR(int, i, 1, n + 2) FOR(int, j, 1, n) EFOR(int, k, 0, j) if (f[i - 1][k] < INF)
			f[i][j] = min(f[i][j], f[i - 1][k] + C(k + 1, j));

		// 1) dncDP, K in [0, n + 2] (K > n: all INF), every j
		int K = rnd(0, n + 2);
		if (dncDP(n, K, C) != f[K]) fail("dncDP", seed);

		// 2) knuth vs O(n^3); also the base-folding rule: dp[i][i] = b_i  ==  knuth + sum b over [l, r]
		{
			auto dp = knuth(n, C);
			ll B = vector<ll>{0, 5, (ll)1e9}[rnd(0, 2)];
			vector<ll> b(n + 1), sb(n + 1, 0);
			FOR(int, i, 1, n) b[i] = rnd(-B, B), sb[i] = sb[i - 1] + b[i];
			vector<vector<ll>> bf(n + 1, vector<ll>(n + 1));
			REV(int, l, n, 1) FOR(int, r, l, n){
				if (l == r){ bf[l][r] = b[l]; continue; }
				bf[l][r] = INF;
				EFOR(int, k, l, r) bf[l][r] = min(bf[l][r], bf[l][k] + bf[k + 1][r] + C(l, r));
			}
			FOR(int, l, 1, n) FOR(int, r, l, n) if (dp[l][r] + sb[r] - sb[l - 1] != bf[l][r]) fail("knuth", seed);
		}

		// 3) aliens: exactly k groups of a[1..n]; f[k][n] must be convex (sanity)
		FOR(int, k, 2, n - 1) if (f[k - 1][n] + f[k + 1][n] < 2 * f[k][n]) fail("not convex (test bug)", seed);
		ll L = rnd(0, 3) ? (ll)1e15 : (ll)7e17;   // 7e17 * 12 + 1.5e14 < 9.2e18
		auto solve = [&](ll lam){
			vector<pair<ll, ll>> g(n + 1, {INF, 0});
			g[0] = {0, 0};
			FOR(int, j, 1, n) EFOR(int, k, 0, j) g[j] = min(g[j], {g[k].fi + C(k + 1, j) + lam, g[k].se + 1});
			return g[n];
		};
		FOR(int, k, 1, n){
			ll dl = k > 1 ? f[k - 1][n] - f[k][n] : 0, dr = k < n ? f[k][n] - f[k + 1][n] : 0;
			ll got;
			int mode = rnd(0, 2);
			if (mode == 0 && max(llabs(dl), llabs(dr)) <= (ll)1e12) got = aliens(k, solve); // default range
			else if (mode == 1){ // tight range
				ll lo = k > 1 ? dl - rnd(0, 2) : -L, hi = k < n ? max(lo, dr + rnd(0, 2)) : L;
				got = aliens(k, solve, lo, hi);
			}
			else got = aliens(k, solve, -L, L);
			if (got != f[k][n]){
				printf("FAIL aliens seed %d n %d k %d got %lld exp %lld\n", seed, n, k, got, f[k][n]);
				return 1;
			}
		}
	}
	puts("ok");
}
