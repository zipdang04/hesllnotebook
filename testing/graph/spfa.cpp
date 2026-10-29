#include "../../tex/contents/graph/spfa.hpp"
// Library Checker shortest_path
int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m, s, t; cin >> n >> m >> s >> t;
	vector<vector<array<ll, 3>>> g(n); vector<int> from(m);
	FOR(int, i, 0, m - 1) { int u, v, w; cin >> u >> v >> w; g[u].push_back({v, w, i}); from[i] = u; }
	mt19937_64 mt(CURRENT_TIMESTAMP);
	for (auto &e: g) shuffle(e.begin(), e.end(), mt);
	SPFA sp;
	sp.run(n, s, [&](int u, auto relax) { for (auto [v, w, id]: g[u]) relax(v, w, id); });
	if (sp.dist[t] >= SPFA::oo) { cout << -1; return 0; }
	vector<int> path {t};
	for (int v = t; v != s; ) path.push_back(v = from[sp.par[v]]);
	reverse(path.begin(), path.end());
	cout << sp.dist[t] << ' ' << (int)path.size() - 1 << '\n';
	for (int i = 1; i < (int)path.size(); i++) cout << path[i-1] << ' ' << path[i] << '\n';
}
