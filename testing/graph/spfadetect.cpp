#include "../../tex/contents/graph/spfa.hpp"

int main() {
	mt19937 rng(42);
	for (int it = 0; it < 50000; it++) {
		int n = rng() % 7 + 1, m = rng() % 12;
		vector<array<ll, 3>> e;
		vector<vector<array<ll, 3>>> g(n);
		for (int i = 0; i < m; i++) {
			int u = rng() % n, v = rng() % n;
			ll w = (ll)(rng() % 21) - 6;
			e.push_back({u, v, w});
			g[u].push_back({v, w, i});
		}
		int s = rng() % n;
		SPFA sp;
		bool neg = sp.run(n, s, [&](int u, auto relax) {
			for (auto [v, w, id]: g[u]) relax(v, w, id);
		}, true);

		// Bellman-Ford reference
		const ll INF = SPFA::oo;
		vector<ll> d(n, INF);
		d[s] = 0;
		for (int k = 0; k < n; k++) {
			for (auto [u, v, w]: e) {
				if (d[u] < INF && d[u] + w < d[v]) d[v] = d[u] + w;
			}
		}
		vector<int> bad(n);
		for (int k = 0; k < n; k++) {
			for (auto [u, v, w]: e) {
				if (d[u] < INF && (d[u] + w < d[v] || bad[u])) {
					bad[v] = 1;
					d[v] = min(d[v], d[u] + w);
				}
			}
		}
		bool bn = false;
		for (int v = 0; v < n; v++) if (bad[v]) bn = true;

		bool ok = (bn == neg);
		for (int v = 0; v < n && ok; v++) {
			ll want = bad[v] ? -INF : d[v];
			if (sp.dist[v] != want) ok = false;
		}
		if (!ok) {
			printf("FAIL it %d n %d s %d neg %d bn %d\n", it, n, s, neg, bn);
			return 1;
		}
	}
	puts("spfa detect ok");
	return 0;
}
