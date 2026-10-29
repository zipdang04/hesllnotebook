#pragma once
#include "../contest/template.hpp"
// pos: 1-indexed preorder (heavy child first); heavy paths and subtrees contiguous
struct HLD{
	int n; vector<vector<int>> g;
	vector<int> par, dep, sz, hv, head, pos;
	HLD(int n): n(n), g(n), par(n, -1), dep(n), sz(n, 1), hv(n, -1), head(n), pos(n){}
	void addEdge(int u, int v){ g[u].push_back(v); g[v].push_back(u); }
	void build(int r = 0){
		vector<int> ord{r};
		FOR(int, i, 0, (int)ord.size() - 1){ int u = ord[i]; for (int v: g[u]) if (v != par[u]) par[v] = u, dep[v] = dep[u] + 1, ord.push_back(v); }
		REV(int, i, (int)ord.size() - 1, 1){
			int u = ord[i], p = par[u]; sz[p] += sz[u];
			if (hv[p] < 0 || sz[u] > sz[hv[p]]) hv[p] = u;
		}
		vector<int> st{r}; head[r] = r; int t = 1;
		while (!st.empty()){ // stack preorder, heavy child popped right after its parent
			int u = st.back(); st.pop_back(); pos[u] = t++;
			for (int v: g[u]) if (v != par[u] && v != hv[u]) head[v] = v, st.push_back(v);
			if (hv[u] >= 0) head[hv[u]] = head[u], st.push_back(hv[u]);
		}
	}
	// f(l, r) on inclusive pos ranges [l, r] covering path u-v (O(log n) calls, any order);
	// edge: skip lca (edge (par[c], c) stored at pos[c])
	template<class F> void path(int u, int v, F f, bool edge = false){
		for (;; v = par[head[v]]){
			if (pos[u] > pos[v]) swap(u, v);
			if (head[u] == head[v]) break;
			f(pos[head[v]], pos[v]);
		}
		if (pos[u] + edge <= pos[v]) f(pos[u] + edge, pos[v]);
	}
	int lca(int u, int v){
		for (;; v = par[head[v]]){ if (pos[u] > pos[v]) swap(u, v); if (head[u] == head[v]) return u; }
	}
	pair<int, int> subtree(int v, bool edge = false){ return {pos[v] + edge, pos[v] + sz[v] - 1}; }
};
