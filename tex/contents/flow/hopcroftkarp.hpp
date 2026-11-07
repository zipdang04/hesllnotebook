#pragma once
#include "../contest/template.hpp"
struct HopcroftKarp{
	int n;
	vector<vector<int>> g;
	vector<int> matchL, matchR, dist;
	HopcroftKarp(int n, int m):
		n(n), g(n), matchL(n, -1), matchR(m, -1), dist(n){}
	void addEdge(int u, int v){ g[u].push_back(v); }
	bool dfs(int u){
		int t = dist[u] + 1;
		// dist[u] = 0 marks u dead for this phase
		dist[u] = 0;
		for (int v: g[u])
			if (matchR[v] == -1 ||
				(dist[matchR[v]] == t && dfs(matchR[v]))){
				matchL[u] = v; matchR[v] = u;
				return true;
			}
		return false;
	}
	int run(){
		int res = 0;
		for (;;){
			dist.assign(n, 0);
			vector<int> q(n);
			int t = 0;
			bool f = false;
			FOR(int, i, 0, n - 1)
				if (matchL[i] == -1){
					q[t++] = i;
					dist[i] = 1;
				}
			for (int i = 0; i < t; i++)
				for (int v: g[q[i]])
					if (matchR[v] == -1) f = true;
					else if (!dist[matchR[v]]){
						dist[matchR[v]] = dist[q[i]] + 1;
						q[t++] = matchR[v];
					}
			if (!f) return res;
			FOR(int, i, 0, n - 1)
				if (matchL[i] == -1) res += dfs(i);
		}
	}
	pair<vector<int>, vector<int>> cover(){
		vector<int> a, b;
		FOR(int, u, 0, n - 1)
			if (!dist[u]) a.push_back(u);
		FOR(int, v, 0, (int)matchR.size() - 1)
			if (matchR[v] != -1 && dist[matchR[v]])
				b.push_back(v);
		return {a, b};
	}
};
