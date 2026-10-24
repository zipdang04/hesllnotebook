#pragma once
#include "../contest/template.hpp"

struct HopcroftKarp{
	int n; vector<vector<int>> g; vector<int> l, r, d; // l[u] / r[v]: partner, -1 if free
	HopcroftKarp(int n, int m): n(n), g(n), l(n, -1), r(m, -1), d(n){}
	void addEdge(int u, int v){ g[u].push_back(v); } // left u in [0,n), right v in [0,m)
	bool dfs(int u){
		int t = exchange(d[u], 0) + 1; // d[u] = 0: dead for this phase
		for (int v: g[u])
			if (r[v] == -1 || (d[r[v]] == t && dfs(r[v]))){ l[u] = v; r[v] = u; return true; }
		return false;
	}
	int run(){ // max matching size (extra pairs if called again)
		int res = 0;
		for (;;){
			d.assign(n, 0);
			int t = 0; bool f = false; vector<int> q(n);
			for (int i = 0; i < n; i++) if (l[i] == -1) q[t++] = i, d[i] = 1;
			for (int i = 0; i < t; i++)
				for (int v: g[q[i]])
					if (r[v] == -1) f = true;
					else if (!d[r[v]]) d[r[v]] = d[q[i]] + 1, q[t++] = r[v];
			if (!f) return res;
			for (int i = 0; i < n; i++) if (l[i] == -1) res += dfs(i);
		}
	}
	pair<vector<int>, vector<int>> cover(){ // Konig min vertex cover {left, right}, after run()
		vector<int> a, b;
		for (int u = 0; u < n; u++) if (!d[u]) a.push_back(u);
		for (int v = 0; v < (int)r.size(); v++) if (r[v] != -1 && d[r[v]]) b.push_back(v);
		return {a, b};
	}
};
