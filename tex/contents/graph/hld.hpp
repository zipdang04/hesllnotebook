#pragma once
#include "../contest/template.hpp"

// Heavy-light decomposition, vertices 1..n, pos in 1..n;
// subtree of u = [pos, pos + sub - 1]. O(n) build, O(log n)
// ranges per path. Two recursive DFS, depth <= V.
struct HLD{
	int timer = 0;
	vector<int> par, depth, sub, heavy, head, pos;
	HLD(const vector<vector<int>> &g, int root = 1):
			par(sz(g)), depth(sz(g)), sub(sz(g)),
			heavy(sz(g)), head(sz(g)), pos(sz(g)){
		dfsSize(root, g); dfsHld(root, root, g);
	}
	void dfsSize(int u, const vector<vector<int>> &g){
		sub[u] = 1;
		for(int v: g[u]) if(v != par[u]){
			par[v] = u; depth[v] = depth[u] + 1;
			dfsSize(v, g); sub[u] += sub[v];
			if(sub[v] > sub[heavy[u]]) heavy[u] = v;
		}
	}
	void dfsHld(int u, int h, const vector<vector<int>> &g){
		head[u] = h; pos[u] = ++timer;
		if(heavy[u])
			dfsHld(heavy[u], h, g);
		for(int v: g[u]) if(v != par[u] && v != heavy[u])
			dfsHld(v, v, g);
	}
	int lca(int u, int v){
		while(head[u] != head[v]){
			if(depth[head[u]] > depth[head[v]])
				swap(u, v);
			v = par[head[v]];
		}
		return depth[u] < depth[v] ? u : v;
	}
	vector<pair<int, int>> path(int u, int v,
			bool edge = false){
		vector<pii> res;
		while(head[u] != head[v]){
			if(depth[head[u]] > depth[head[v]])
				swap(u, v);
			res.push_back({pos[head[v]], pos[v]});
			v = par[head[v]];
		}
		if(depth[u] > depth[v])
			swap(u, v);
		if(pos[u] + edge <= pos[v])
			res.push_back({pos[u] + edge, pos[v]});
		return res;
	}
};
