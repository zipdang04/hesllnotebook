#include "../../tex/contents/graph/maxclique.hpp"
#include <cassert>

int main(){
	assert(MaxClique(vector<vector<int>>(1)).clique.empty());
	assert(MaxClique(vector<vector<int>>(2)).clique
		== vector<int>{1});
	assert(sz(MaxClique(vector<vector<int>>(6)).clique) == 1);
	vector<vector<int>> g = {{}, {2, 3}, {1, 3},
		{1, 2, 4}, {3}};
	MaxClique mc(g);
	auto clique = mc.clique;
	sort(all(clique));
	assert((clique == vector<int>{1, 2, 3}));
	g[0] = {1, 2};
	MaxClique other(g);
	clique = other.clique; sort(all(clique));
	assert((clique == vector<int>{1, 2, 3}));
	vector<vector<int>> g2 = {{}, {1, 2, 2}, {1, 1}, {}};
	mc = MaxClique(g2);
	clique = mc.clique; sort(all(clique));
	assert((clique == vector<int>{1, 2}));
	clique = other.clique; sort(all(clique));
	assert((clique == vector<int>{1, 2, 3}));
	vector<vector<int>> complete(128);
	FOR(u, 1, 127) FOR(v, 1, 127) if(u != v)
		complete[u].push_back(v);
	MaxClique full(complete);
	clique = full.clique; sort(all(clique));
	assert(sz(clique) == 127);
	FOR(u, 1, 127)
		assert(clique[u-1] == u);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<vector<char>> adj(n + 1, vector<char>(n + 1));
	FOR(i, 1, m){
		int u, v; cin >> u >> v; u++; v++;
		adj[u][v] = adj[v][u] = 1;
	}
	g.assign(n + 1, {});
	FOR(u, 1, n) FOR(v, 1, n) if(u != v && !adj[u][v])
		g[u].push_back(v);
	mc = MaxClique(g);
	vector<char> seen(n + 1);
	for(int u: mc.clique){
		assert(1 <= u && u <= n && !seen[u]);
		seen[u] = 1;
		for(int v: mc.clique) if(u != v)
			assert(!adj[u][v]);
	}
	cout << sz(mc.clique) << '\n';
	FOR(i, 0, sz(mc.clique) - 1)
		cout << mc.clique[i] - 1
			<< (i + 1 == sz(mc.clique) ? '\n' : ' ');
}
