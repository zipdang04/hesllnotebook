#include "../../tex/contents/flow/stoerwagner.hpp"

mt19937_64 mt;
ll rnd(ll a, ll b) { return uniform_int_distribution<ll>(a, b)(mt); }

int main() {
	for (int seed = 0; seed < 50000; seed++) {
		mt.seed(seed);
		int n = seed % 10 == 0 ? 2 : rnd(2, 12);
		ll W = vector<ll>{0, 1, 10, (ll)1e9}[rnd(0, 3)];
		int m = rnd(0, 3 * n), k = 0;
		if (rnd(0, 3) == 0) k = rnd(1, n - 1);
		vector<array<ll, 3>> e;
		FOR(int, i, 0, m) {
			int u = rnd(0, n - 1), v = rnd(0, n - 1);
			if (k && (u < k) != (v < k)) continue;
			e.push_back({u, v, rnd(0, W)});
			if (rnd(0, 4) == 0) e.push_back({u, v, rnd(0, W)});
		}
		auto cutw = [&](int mask) {
			ll r = 0;
			for (auto &[u, v, w] : e) if ((mask >> u & 1) != (mask >> v & 1)) r += w;
			return r;
		};
		ll exp = LLONG_MAX;
		for (int mask = 1; mask < (1 << n) - 1; mask += 2) exp = min(exp, cutw(mask));
		StoerWagner sw(n);
		for (auto &[u, v, w] : e) sw.addEdge(u, v, w);
		auto [got, side] = sw.run();
		int sm = 0; bool ok = got == exp;
		for (int x : side) {
			if (x < 0 || x >= n || (sm >> x & 1)) ok = false;
			else sm |= 1 << x;
		}
		if (ok && (sm == 0 || sm == (1 << n) - 1 || cutw(sm) != got)) ok = false;
		if (!ok) {
			printf("FAIL seed %d n %d got %lld expected %lld\nside:", seed, n, got, exp);
			for (int x : side) printf(" %d", x);
			printf("\nedges:\n");
			for (auto &[u, v, w] : e) printf("%d %d %lld\n", (int)u, (int)v, w);
			return 1;
		}
	}
	auto r = StoerWagner(1).run();
	if (r.fi != 0 || !r.se.empty()) { puts("FAIL n=1"); return 1; }
	puts("ok");
}
