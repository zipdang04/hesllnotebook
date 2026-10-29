#pragma once
#include "../contest/template.hpp"

struct Dominator{
	int n; vector<vector<int>> g, rg;
	Dominator(int n): n(n), g(n), rg(n) {}
	void addEdge(int u, int v){ g[u].push_back(v); rg[v].push_back(u); }
	vector<int> run(int s){ // idom[v] (idom[s] = s), -1 if v unreachable from s
		vector<int> id(n, -1), rv, par; vector<pair<int, int>> st{{s, -1}};
		while (!st.empty()){ // iterative DFS, vertices renamed to preorder 0..t-1
			auto [v, p] = st.back(); st.pop_back();
			if (id[v] >= 0) continue;
			id[v] = rv.size(); rv.push_back(v); par.push_back(p);
			for (int u: g[v]) if (id[u] < 0) st.push_back({u, id[v]});
		}
		int t = rv.size(); vector<int> sd(t), dsu(t), mn(t), dom(t), q; vector<vector<int>> bk(t);
		iota(sd.begin(), sd.end(), 0); dsu = mn = sd;
		auto eval = [&](int v){ // min-sdom vertex on v's DSU path (iterative path compression)
			int r = v; while (dsu[r] != r) q.push_back(r), r = dsu[r];
			while (!q.empty()){
				int x = q.back(), p = dsu[x]; q.pop_back();
				if (sd[mn[p]] < sd[mn[x]]) mn[x] = mn[p];
				dsu[x] = r;
			}
			return mn[v];
		};
		REV(int, w, t - 1, 1){
			for (int u: rg[rv[w]]) if (id[u] >= 0) sd[w] = min(sd[w], sd[eval(id[u])]);
			bk[sd[w]].push_back(w); int p = dsu[w] = par[w];
			for (int v: bk[p]){ int u = eval(v); dom[v] = sd[u] < p ? u : p; }
			bk[p].clear();
		}
		vector<int> idom(n, -1);
		FOR(int, w, 0, t - 1){ if (w && dom[w] != sd[w]) dom[w] = dom[dom[w]]; idom[rv[w]] = rv[dom[w]]; }
		return idom;
	}
};
