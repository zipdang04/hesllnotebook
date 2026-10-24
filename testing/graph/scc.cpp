#include "../../tex/contents/graph/scc.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cin >> n >> m;
	vector<pair<int, int>> es(m);
	FOR(int, i, 0, m - 1) {
		int a, b; cin >> a >> b;
		graph[a].push_back(Node(b, i));
		es[i] = {a, b};
	}
	FOR(int, u, 0, n - 1) if (!Tarjan::num[u]) Tarjan::dfs(u);

	// condensation, then topological order (the judge requires it)
	vector<vector<int>> cond(n); vector<int> deg(n, 0);
	for (auto [a, b]: es) {
		int ra = Tarjan::root[a], rb = Tarjan::root[b];
		if (ra != rb) cond[ra].push_back(rb), deg[rb]++;
	}
	queue<int> q;
	FOR(int, u, 0, n - 1)
		if (!Tarjan::scc[u].empty() && !deg[u]) q.push(u);
	vector<int> order;
	while (!q.empty()) {
		int r = q.front(); q.pop(); order.push_back(r);
		for (int v: cond[r]) if (!--deg[v]) q.push(v);
	}

	cout << order.size() << '\n';
	for (int r: order) {
		cout << Tarjan::scc[r].size();
		for (int v: Tarjan::scc[r]) cout << ' ' << v;
		cout << '\n';
	}
}
