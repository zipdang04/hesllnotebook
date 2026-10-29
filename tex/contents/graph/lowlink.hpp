#pragma once
#include "../contest/template.hpp"
struct Lowlink{
	int n, m = 0, t = 0, k2 = 0;
	vector<vector<pair<int,int>>> g;
	vector<int> num, low, st, st2, two; // two[v]: 2-edge-connected component id
	vector<bool> isArt, isBridge; // isBridge[edge id]
	vector<vector<int>> bcc; // blocks as vertex lists (isolated vertex = own block)
	Lowlink(int n): n(n), g(n), num(n), low(n), two(n), isArt(n){}
	void addEdge(int u, int v){ g[u].push_back({v, m}); g[v].push_back({u, m++}); isBridge.push_back(0); }
	void dfs(int u, int pe){
		num[u] = low[u] = ++t; st.push_back(u); st2.push_back(u);
		int ch = 0;
		for (auto [v, e]: g[u]) if (e != pe){
			if (num[v]){ low[u] = min(low[u], num[v]); continue; }
			dfs(v, e); low[u] = min(low[u], low[v]); ch++;
			if (low[v] > num[u]) isBridge[e] = 1;
			if (low[v] >= num[u]){
				if (pe >= 0 || ch > 1) isArt[u] = 1;
				bcc.push_back({u}); int x;
				do x = st.back(), st.pop_back(), bcc.back().push_back(x); while (x != v);
			}
		}
		if (pe < 0 && !ch) bcc.push_back({u});
		if (low[u] == num[u]){ int x; do x = st2.back(), st2.pop_back(), two[x] = k2; while (x != u); k2++; }
	}
	void run(){ FOR(int, i, 0, n - 1) if (!num[i]) dfs(i, -1), st.clear(); }
};
