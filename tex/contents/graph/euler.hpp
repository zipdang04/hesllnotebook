#pragma once
#include "../contest/template.hpp"

// Euler walk from s, directed/undirected, O(V + E).
// Vertices 1..n, g[u] = {v, id}, ids in 0..m;
// undirected: each edge in both lists with the same id.
vector<pair<int, int>> eulerWalk(
		const vector<vector<pair<int, int>>> &g, int m, int s){
	vector<int> D(sz(g)), its(sz(g)); vector<char> eu(m + 1);
	vector<pii> ret, stk = {{s, -1}};
	D[s]++; // to allow Euler paths, not just cycles
	while(!stk.empty()){
		int x = stk.back().fi; int &it = its[x];
		if(it == sz(g[x])){
			ret.push_back(stk.back()); stk.pop_back(); continue;
		}
		auto [y, e] = g[x][it++];
		if(eu[e]) continue;
		D[x]--, D[y]++; eu[e] = 1; stk.push_back({y, e});
	}
	if(sz(ret) != m + 1) return {};
	for(int x: D) if(x < 0)
		return {};
	return {ret.rbegin(), ret.rend()};
}
