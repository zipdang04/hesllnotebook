#include "../../tex/contents/flow/blossom.hpp"
#include <cassert>

int main(){
	vector<vector<int>> g0(1);
	assert(GeneralMatching(g0).pairs == 0);
	{
		vector<vector<int>> g(2);
		g[1] = {1};
		GeneralMatching gm(g);
		assert(gm.pairs == 0 && gm.match[1] == 0);
	}
	vector<vector<int>> g(5);
	g[0] = {1};
	auto add = [&](int u, int v){
		g[u].push_back(v); g[v].push_back(u);
	};
	add(1, 2); add(2, 3); add(1, 3); add(3, 4); add(1, 2);
	g[2].push_back(2);
	GeneralMatching gm(g);
	assert(gm.pairs == 2 && gm.match[4] == 3);
	assert(gm.match[gm.match[1]] == 1);
	{
		vector<vector<int>> g2(3);
		g2[1] = {2}; g2[2] = {1};
		GeneralMatching hm(g2);
		assert(hm.pairs == 1 && gm.pairs == 2);
		gm = GeneralMatching(g2);
		assert(gm.pairs == 1 && gm.match[1] == 2);
		assert(hm.match[2] == 1);
	}
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<vector<int>> h(n + 1);
	vector<vector<char>> adj(n + 1, vector<char>(n + 1));
	FOR(i, 1, m){
		int u, v; cin >> u >> v;
		u++, v++;
		h[u].push_back(v); h[v].push_back(u);
		adj[u][v] = adj[v][u] = 1;
	}
	GeneralMatching res(h);
	int cnt = 0;
	FOR(u, 1, n) if(res.match[u]){
		assert(res.match[res.match[u]] == u);
		assert(adj[u][res.match[u]]);
		cnt++;
	}
	assert(cnt == 2 * res.pairs);
	cout << res.pairs << '\n';
	FOR(u, 1, n) if(res.match[u] > u)
		cout << u - 1 << ' ' << res.match[u] - 1 << '\n';
}
