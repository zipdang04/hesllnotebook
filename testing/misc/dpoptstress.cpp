#include "../../tex/contents/misc/dpopt.hpp"

mt19937_64 mt;
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
const ll INF = LLONG_MAX / 4;
void fail(const char *what, int seed){ printf("FAIL %s seed %d\n", what, seed); exit(1); }

int main(){
	for (int seed = 0; seed < 200000; seed++){
		mt.seed(seed);
		int n = seed % 7 == 0 ? rnd(1, 3) : rnd(1, 12);
		// cost of the closed interval [l, r] of items 1..n: sum of w over sub-intervals (QI + monotone)
		// or (a_l + .. + a_r)^2 with a >= 0 (QI + monotone); many zeros give ties
		int fam = rnd(0, 1);
		ll W = vector<ll>{0, 1, 10, (ll)1e6}[rnd(0, 3)];
		vector<vector<ll>> w(n + 2, vector<ll>(n + 2, 0));
		vector<ll> S(n + 1, 0);
		FOR(int, i, 1, n) S[i] = S[i - 1] + (rnd(0, 2) ? rnd(0, W) : 0);
		FOR(int, l, 1, n) FOR(int, r, l, n) if (rnd(0, 3) == 0) w[l][r] = rnd(0, W);
		vector<vector<ll>> I(n + 2, vector<ll>(n + 2, 0)); // I[l][r], l <= r
		REV(int, l, n, 1) FOR(int, r, l, n){
			ll s = w[l][r];
			if (r > l) s += I[l + 1][r] + I[l][r - 1] - (l + 1 <= r - 1 ? I[l + 1][r - 1] : 0);
			I[l][r] = s;
		}
		auto cost = [&](int l, int r)->ll{ return fam ? I[l][r] : (S[r] - S[l - 1]) * (S[r] - S[l - 1]); };
		auto C = [&](int k, int j){ return cost(k + 1, j); }; // group (k, j]

		// brute: f[i][j] = min cost of splitting items 1..j into i groups
		vector<vector<ll>> f(n + 1, vector<ll>(n + 1, INF));
		f[0][0] = 0;
		FOR(int, i, 1, n) FOR(int, j, i, n) EFOR(int, k, i - 1, j) if (f[i - 1][k] < INF)
			f[i][j] = min(f[i][j], f[i - 1][k] + C(k, j));

		// 1) dnc layers
		int K = rnd(1, n);
		vector<ll> prev(n + 1, INF), cur(n + 1, INF);
		FOR(int, j, 1, n) prev[j] = C(0, j);
		FOR(int, j, 1, n) if (prev[j] != f[1][j]) fail("dnc layer1(brute)", seed);
		FOR(int, i, 2, K){
			fill(cur.begin(), cur.end(), INF);
			dnc(i, n, i - 1, n - 1, prev, cur, C);
			FOR(int, j, i, n) if (cur[j] != f[i][j]) fail("dnc", seed);
			swap(prev, cur);
		}

		// 2) knuth on n items (0-indexed), interval cost family (QI + monotone), random base
		{
			ll B = vector<ll>{0, 5, (ll)1e9}[rnd(0, 2)];
			vector<ll> base(n);
			for (ll &x: base) x = rnd(-B, B);
			auto Ck = [&](int l, int r){ return I[l + 1][r + 1]; };
			auto dp = knuth(n, Ck, [&](int i){ return base[i]; });
			vector<vector<ll>> bf(n, vector<ll>(n));
			REV(int, l, n - 1, 0) EFOR(int, r, l, n){
				if (l == r){ bf[l][r] = base[l]; continue; }
				bf[l][r] = INF;
				EFOR(int, k, l, r) bf[l][r] = min(bf[l][r], bf[l][k] + bf[k + 1][r] + Ck(l, r));
			}
			if (dp != bf) fail("knuth", seed);
		}

		// 3) aliens: exactly k groups of items 1..n; f[k][n] must be convex (sanity)
		FOR(int, k, 2, n - 1) if (f[k - 1][n] + f[k + 1][n] < 2 * f[k][n]) fail("not convex (test bug)", seed);
		ll L = rnd(0, 3) ? (ll)1e15 : (ll)7e17;   // 7e17 * 12 = 8.4e18 < 9.2e18
		auto solve = [&](ll lam){
			vector<pair<ll, ll>> g(n + 1, {INF, 0});
			g[0] = {0, 0};
			FOR(int, j, 1, n) EFOR(int, k, 0, j) g[j] = min(g[j], {g[k].fi + C(k, j) + lam, g[k].se + 1});
			return g[n];
		};
		FOR(int, k, 1, n){
			ll lo = -L, hi = L;
			if (rnd(0, 1)){ // tight: lo <= f(k-1) - f(k), hi >= f(k) - f(k+1)
				if (k > 1) lo = f[k - 1][n] - f[k][n] - rnd(0, 2);
				if (k < n) hi = max(lo, f[k][n] - f[k + 1][n] + rnd(0, 2));
			}
			ll got = aliens(lo, hi, k, solve);
			if (got != f[k][n]){
				printf("FAIL aliens seed %d n %d k %d lo %lld hi %lld got %lld exp %lld\n", seed, n, k, lo, hi, got, f[k][n]);
				return 1;
			}
		}
	}
	puts("ok");
}
