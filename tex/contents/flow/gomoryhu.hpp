#pragma once
#include "dinic.hpp"

// Gomory-Hu tree (Gusfield): n - 1 Dinic runs, vertices
// 1..n. Min u-v cut = least weight on the tree path u..v.
// Recursion: Dinic's, depth <= V.
vector<array<ll, 3>> gomoryHu(
		const vector<vector<pair<int, ll>>> &g){
	int n = sz(g) - 1;
	Dinic base(n + 1);
	FOR(u, 1, n) for(auto [v, w]: g[u]) base.addEdge(u, v, w);
	vector<int> par(n + 1, 1);
	vector<array<ll, 3>> tree;
	FOR(i, 2, n){
		Dinic d = base;
		tree.push_back({i, par[i], d.maxFlow(i, par[i])});
		FOR(j, i + 1, n) if(par[j] == par[i] && d.sideS(j))
			par[j] = i;
	}
	return tree;
}
