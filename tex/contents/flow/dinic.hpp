#pragma once
#include "../contest/template.hpp"

// Max flow: Dinic + capacity scaling, O(VE log U); unit
// bipartite graph O(E sqrt V).
struct Dinic{
	struct Edge{ int to; ll cap; };
	vector<Edge> edges; // edges[id ^ 1] = reverse of id
	vector<vector<int>> adj; // ids of arcs out of u
	vector<int> level, ptr;
	Dinic(int n_): adj(n_), level(n_), ptr(n_){}
	// rcap = cap: undirected edge. Returns the arc id.
	int addEdge(int u, int v, ll cap, ll rcap = 0){
		adj[u].push_back(sz(edges)), edges.push_back({v, cap});
		adj[v].push_back(sz(edges)), edges.push_back({u, rcap});
		return sz(edges) - 2;
	}
	bool bfs(int s, int t, ll lim){
		fill(all(level), -1);
		vector<int> q = {t};
		level[t] = 0;
		for(int i = 0; i < sz(q) && level[s] < 0; i++){
			int v = q[i];
			for(int id: adj[v]){
				int u = edges[id].to;
				if(level[u] >= 0 || edges[id ^ 1].cap < lim)
					continue;
				level[u] = level[v] + 1, q.push_back(u);
			}
		}
		return level[s] >= 0;
	}
	ll dfs(int u, int t, ll f, ll lim){
		if(u == t) return f;
		ll pushed = 0;
		for(int &i = ptr[u]; i < sz(adj[u]); i++){
			int id = adj[u][i], v = edges[id].to;
			if(edges[id].cap < lim || level[v] != level[u] - 1)
				continue;
			ll room = min(f - pushed, edges[id].cap) / lim * lim;
			ll got = dfs(v, t, room, lim);
			if(!got) continue;
			edges[id].cap -= got, edges[id ^ 1].cap += got;
			pushed += got;
			if(pushed == f) return pushed;
		}
		return pushed;
	}
	ll maxFlow(int s, int t){
		if(s == t) return 0;
		ll ans = 0;
		for(ll lim = 1LL << 60; lim; lim >>= 1)
			while(bfs(s, t, lim)){
				fill(all(ptr), 0);
				ans += dfs(s, t, LLONG_MAX, lim);
			}
		return ans;
	}
	ll flowOf(int id){ return edges[id ^ 1].cap; }
	bool sideS(int v){ return level[v] < 0; }
};
