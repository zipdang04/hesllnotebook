#pragma once
#include "../contest/template.hpp"

// Bridges, cut vertices, BCC (edges) of undirected graph.
// O(V + E). Vertices 1..n, g[u] = {v, edge id}, ids 0..m.
// Recursive, depth <= V.
struct LowLink{
	int bccCnt = 0, timer = 0;
	vector<int> bcc, num, low, stk;
	vector<bool> isBridge, isCut;
	LowLink(const vector<vector<pair<int, int>>> &g, int m):
		bcc(m + 1), num(g.size()), low(g.size()),
		isBridge(m + 1), isCut(g.size()){
		FOR(u, 1, (int)g.size() - 1)
			if(!num[u]) dfs(u, -1, g);
	}
	void dfs(int u, int pe,
		const vector<vector<pair<int, int>>> &g){
		num[u] = low[u] = ++timer;
		int ch = 0;
		for(auto [v, id]: g[u]){
			if(id == pe) continue;
			if(num[v]){
				minimize(low[u], num[v]);
				if(num[v] < num[u]) stk.push_back(id);
				continue;
			}
			stk.push_back(id); dfs(v, id, g); ch++;
			minimize(low[u], low[v]);
			if(low[v] > num[u]) isBridge[id] = true;
			if(low[v] < num[u]) continue;
			if(pe >= 0 || ch > 1) isCut[u] = true;
			bccCnt++;
			while(stk.back() != id){
				bcc[stk.back()] = bccCnt; stk.pop_back();
			}
			bcc[id] = bccCnt; stk.pop_back();
		}
	}
};
