#include "../../tex/contents/flow/mcmf.hpp"
#include "../../tex/contents/flow/costscaling.hpp"

int main() {
	mt19937_64 rng(1337);
	// Part 1: DAG graphs with negative costs, testing calculate(limit) and default calculate(), flowOn conservation, vs costscaling
	for (int it = 0; it < 10000; it++) {
		int n = rng() % 6 + 2, m = rng() % 10;
		MCMF<ll> f;
		f.init(n, 0, n - 1);
		MinCostCirculation mc(n);
		vector<array<ll, 4>> E;
		for (int i = 0; i < m; i++) {
			int u = rng() % n, v = rng() % n;
			if (u == v) continue;
			if (u > v) swap(u, v);
			ll c = rng() % 5, w = (ll)(rng() % 11) - 5;
			E.push_back({u, v, c, w});
			f.addEdge(u, v, c, w);
			mc.addEdge(u, v, 0, c, w);
		}
		// alternate between explicit random limit and default limit (min-cost max-flow)
		bool useDef = (rng() % 2 == 0);
		ll lim = useDef ? (1ll << 62) : (ll)(rng() % 6);
		auto [fl, co] = useDef ? f.calculate() : f.calculate(lim);

		// Check flow conservation and flowOn
		vector<ll> ex(n);
		ll cc = 0;
		for (int i = 0; i < (int)E.size(); i++) {
			ll x = f.flowOn(i);
			if (x < 0 || x > E[i][2]) { puts("flowOn out of bounds"); return 1; }
			ex[E[i][0]] -= x;
			ex[E[i][1]] += x;
			cc += x * E[i][3];
		}
		bool ok = (cc == co && ex[n - 1] == fl && ex[0] == -fl);
		for (int v = 1; v < n - 1; v++) if (ex[v] != 0) ok = false;

		// Compare with circulation forcing flow fl
		mc.addEdge(n - 1, 0, fl, fl, 0);
		auto [feas, c2] = mc.solve();
		if (!feas || (ll)c2 != co) ok = false;
		if (!ok) {
			printf("FAIL part 1 it %d fl %lld co %lld cc %lld c2 %lld feas %d\n", it, fl, co, cc, (ll)c2, feas);
			return 1;
		}
	}

	// Part 2: MCMF<__int128> with large costs (|w| <= 1e9) and large caps (c <= 1e12) vs MinCostCirculation
	for (int it = 0; it < 1000; it++) {
		int n = rng() % 7 + 2, m = rng() % 15;
		MCMF<__int128> f;
		f.init(n, 0, n - 1);
		MinCostCirculation mc(n);
		for (int i = 0; i < m; i++) {
			int u = rng() % n, v = rng() % n;
			if (u == v) continue;
			if (u > v) swap(u, v);
			ll c = rng() % ((ll)1e12), w = (ll)(rng() % ((ll)2e9 + 1)) - (ll)1e9;
			f.addEdge(u, v, c, w);
			mc.addEdge(u, v, 0, c, w);
		}
		auto [fl, co] = f.calculate(); // default limit
		mc.addEdge(n - 1, 0, fl, fl, 0);
		auto [feas, c2] = mc.solve();
		if (!feas || c2 != co) {
			printf("FAIL part 2 it %d fl %lld feas %d\n", it, fl, feas);
			return 1;
		}
	}

	puts("mcmf stress ok");
	return 0;
}
