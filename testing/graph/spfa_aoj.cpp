#include "../../tex/contents/graph/spfa.hpp"
// AOJ GRL_1_B: distances from r, or NEGATIVE CYCLE if one is reachable from r
int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m, r; cin >> n >> m >> r;
	vector<vector<array<ll, 3>>> g(n);
	FOR(int, i, 0, m - 1) { int u, v, w; cin >> u >> v >> w; g[u].push_back({v, w, i}); }
	SPFA sp;
	if (sp.run(n, r, [&](int u, auto relax) { for (auto [v, w, id]: g[u]) relax(v, w, id); })) {
		cout << "NEGATIVE CYCLE\n"; return 0;
	}
	FOR(int, v, 0, n - 1) {
		if (sp.dist[v] >= SPFA::oo) cout << "INF\n";
		else cout << sp.dist[v] << '\n';
	}
}
