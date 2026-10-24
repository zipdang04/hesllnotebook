#pragma once
#include "../contest/template.hpp"

struct Euler{
	int n, m; bool dir; vector<vector<pair<int, int>>> g; vector<int> d;
	void init(int n, bool directed){this->n = n; m = 0; dir = directed; g.assign(n, {}); d.assign(n, 0);}
	int addEdge(int u, int v){ // returns edge id; self-loops, multi-edges ok
		g[u].push_back({v, m}); if (!dir) g[v].push_back({u, m});
		d[u]++; d[v] += dir ? -1 : 1; return m++;
	}
	// vs = v_0..v_m, es = e_0..e_{m-1} (e_i: vs[i] -> vs[i+1]); false if no trail
	bool solve(vector<int> &vs, vector<int> &es){
		int s = 0;
		EFOR(int, v, 0, n) if (g[v].size() && (g[s].empty() || (dir ? d[v] > 0 : d[v] & 1))) s = v;
		vector<int> it(n), D(n), used(m); vector<pair<int, int>> st = {{s, -1}};
		vs.clear(); es.clear(); D[s]++; // allows a path, not only a circuit
		while (st.size()){
			auto [x, e0] = st.back();
			if (it[x] == (int)g[x].size()){vs.push_back(x); if (e0 >= 0) es.push_back(e0); st.pop_back(); continue;}
			auto [y, e] = g[x][it[x]++];
			if (!used[e]) used[e] = 1, D[x]--, D[y]++, st.push_back({y, e});
		}
		for (int x: D) if (x < 0) return false;
		reverse(vs.begin(), vs.end()); reverse(es.begin(), es.end());
		return (int)es.size() == m;
	}
};
