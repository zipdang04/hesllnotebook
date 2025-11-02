#include "../../../tex/contents/graph/shortestpath/spfa.hpp"

int n, m, s, t;
int main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	cin >> n >> m >> s >> t; SPFA::n = n;
	FOR(int, _, 1, m) {
		int u, v, w; cin >> u >> v >> w;
		SPFA::graph[u].emplace_back(v, w);
	}
	SPFA::spfa(s);
	// FOR(int, i, 0, n-1) cerr << SPFA::dist[i] << " \n"[i == n-1];
	if (SPFA::dist[t] >= SPFA::oo) {
		cout << -1; return 0;
	}
	// cerr << "oke\n";
	ll ans = SPFA::dist[t];

	vector<int> path {t};
	int node = t;
	while (node != s) {
		// cerr << node << ' ';
		node = SPFA::trace[node];
		path.push_back(node);
	}
	reverse(path.begin(), path.end());
	cout << ans << ' ' << (int) path.size() - 1 << '\n';
	for (int i = 1; i < (int)path.size(); i++)
		cout << path[i-1] << ' ' << path[i] << "\n";
}
