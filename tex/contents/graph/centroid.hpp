#pragma once
#include "../contest/template.hpp"

struct Centroid{
	int n; vector<vector<int>> g;
	vector<int> par, lev, sz, pa, dep; vector<bool> rem;
	Centroid(int n): n(n), g(n), par(n, -1), lev(n), sz(n), pa(n), dep(n), rem(n) {}
	void addEdge(int u, int v){ g[u].push_back(v); g[v].push_back(u); }
	// non-removed vertices reachable from r without passing p, in BFS order;
	// pa = BFS parent, dep[u] = dist(u, p) (dist(u, r) if p < 0)
	vector<int> bfs(int r, int p){
		vector<int> q{r}; pa[r] = p; dep[r] = p >= 0;
		EFOR(int, i, 0, (int)q.size()){
			int u = q[i];
			for (int v: g[u]) if (v != pa[u] && !rem[v]) pa[v] = u, dep[v] = dep[u] + 1, q.push_back(v);
		}
		return q;
	}
	template<class F> void dec(int r, int p, F &f){
		auto q = bfs(r, -1); int m = q.size(), c = r;
		for (int u: q) sz[u] = 1;
		REV(int, i, m - 1, 1) sz[pa[q[i]]] += sz[q[i]];
		for (int u: q) if (sz[u] * 2 >= m) c = u; // deepest such u is the centroid
		rem[c] = 1; par[c] = p; lev[c] = p < 0 ? 0 : lev[p] + 1; f(c);
		for (int v: g[c]) if (!rem[v]) dec(v, c, f);
	}
	// f(c) once per centroid, top-down; during f(c), rem = {c} + its centroid ancestors (+ other branches)
	template<class F> void run(F f){ EFOR(int, i, 0, n) if (!rem[i]) dec(i, -1, f); }
};
