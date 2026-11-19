#pragma once
#include "../contest/template.hpp"

// Lengauer-Tarjan dominator tree from root; vertices 1..n.
// O(E log V). idom[root] = root; idom[v] = 0: unreachable.
// Recursive find and dfs, depth <= V.
struct Dominator{
	vector<int> idom;
	vector<int> sdom, anc, val, par, ord;
	Dominator(const vector<vector<int>> &g, int root):
			idom(sz(g)), sdom(sz(g), -1), anc(sz(g)),
			val(sz(g)), par(sz(g)){
		int n = sz(g) - 1;
		vector<vector<int>> rg(n + 1), buf(n + 1);
		vector<int> u(n + 1);
		iota(all(anc), 0); iota(all(val), 0);
		FOR(i, 1, n)
			for(int j: g[i])
				rg[j].push_back(i);
		dfs(root, g); reverse(all(ord));
		for(int i: ord){
			for(int j: rg[i]){
				if(sdom[j] == -1) continue;
				find(j); minimize(sdom[i], sdom[val[j]]);
			}
			buf[ord[sz(ord) - sdom[i] - 1]].push_back(i);
			for(int j: buf[par[i]]){
				find(j); u[j] = val[j];
			}
			buf[par[i]].clear();
			anc[i] = par[i];
		}
		reverse(all(ord)); idom[root] = root;
		// idom holds order numbers until the next loop
		for(int i: ord) if(i != root)
			idom[i] = sdom[i] == sdom[u[i]]
				? sdom[i] : idom[u[i]];
		for(int i: ord) if(i != root)
			idom[i] = ord[idom[i]];
	}
	int find(int v){
		if(v == anc[v]) return v;
		int ret = find(anc[v]);
		if(sdom[val[v]] > sdom[val[anc[v]]])
			val[v] = val[anc[v]];
		return anc[v] = ret;
	}
	void dfs(int v, const vector<vector<int>> &g){
		sdom[v] = sz(ord); ord.push_back(v);
		for(int i: g[v]) if(sdom[i] == -1){
			par[i] = v;
			dfs(i, g);
		}
	}
};
