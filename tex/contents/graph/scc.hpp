#pragma once
#include "../contest/template.hpp"

// SCC by Tarjan, O(V + E), vertices 1..n; ids 1..cnt.
// Edge u -> v implies comp[u] >= comp[v] (reverse topo).
// Recursive, depth <= V.
struct SCC{
	int cnt = 0, timer = 0;
	vector<int> comp, num, low, stk;
	SCC(const vector<vector<int>> &g):
			comp(g.size()), num(g.size()), low(g.size()){
		FOR(u, 1, (int)g.size() - 1) if(!num[u]) dfs(u, g);
	}
	void dfs(int u, const vector<vector<int>> &g){
		low[u] = num[u] = ++timer; stk.push_back(u);
		for(int v: g[u]) if(!comp[v]){
			if(!num[v]) dfs(v, g);
			minimize(low[u], low[v]);
		}
		if(low[u] == num[u]){
			cnt++;
			int v;
			do{
				v = stk.back(); stk.pop_back(); comp[v] = cnt;
			}while(v != u);
		}
	}
};
