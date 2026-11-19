#pragma once
#include "../contest/template.hpp"

// Centroid decomposition, vertices 1..n, O(n log n).
// par: parent in the centroid tree, 0 at a top; level
// from 0, <= log2 n. Recursive, depth <= 2V.
struct Centroid{
	vector<int> par, level, sub;
	vector<char> taken;
	Centroid(const vector<vector<int>> &g):
			par(sz(g)), level(sz(g)), sub(sz(g)), taken(sz(g)){
		FOR(u, 1, sz(g) - 1) if(!taken[u])
			build(u, 0, calcSize(u, 0, g), g);
	}
	int calcSize(int u, int p, const vector<vector<int>> &g){
		sub[u] = 1;
		for(int v: g[u]) if(v != p && !taken[v])
			sub[u] += calcSize(v, u, g);
		return sub[u];
	}
	// p: the centroid above; size: the component's size
	void build(int u, int p, int size,
			const vector<vector<int>> &g){
		for(int v: g[u]) if(!taken[v] && sub[v] * 2 >= size){
			// blocks the walk from coming back to u
			sub[u] = 0;
			build(v, p, size, g);
			return;
		}
		taken[u] = 1, par[u] = p;
		level[u] = p ? level[p] + 1 : 0;
		for(int v: g[u]) if(!taken[v])
			build(v, u, calcSize(v, 0, g), g);
	}
};
