#pragma once
#include "../contest/template.hpp"
struct Dinic{
	struct Edge{ int to, rev; ll cap; };
	static constexpr ll oo = 1ll << 60;
	int n, s, t;
	vector<vector<Edge>> g;
	vector<pair<int, int>> at;
	vector<int> level, ptr;
	Dinic(int n): n(n), g(n), level(n), ptr(n){}
	int addEdge(int u, int v, ll c){
		at.push_back({u, (int)g[u].size()});
		g[u].push_back({v, (int)g[v].size(), c});
		g[v].push_back({u, (int)g[u].size() - 1, 0});
		return (int)at.size() - 1;
	}
	// level[v] = residual distance from v to t
	bool bfs(ll lim){
		fill(level.begin(), level.end(), -1);
		queue<int> q({t});
		level[t] = 0;
		while (!q.empty() && level[s] < 0){
			int v = q.front(); q.pop();
			for (Edge &e: g[v]){
				if (level[e.to] >= 0) continue;
				if (g[e.to][e.rev].cap < lim) continue;
				level[e.to] = level[v] + 1;
				q.push(e.to);
			}
		}
		return level[s] >= 0;
	}
	ll dfs(int v, ll f, ll lim){
		if (v == t) return f;
		ll pushed = 0;
		for (int &i = ptr[v]; i < (int)g[v].size(); i++){
			Edge &e = g[v][i];
			if (e.cap < lim) continue;
			if (level[e.to] != level[v] - 1) continue;
			ll x = min(f - pushed, e.cap) / lim * lim;
			ll got = dfs(e.to, x, lim);
			e.cap -= got; g[e.to][e.rev].cap += got;
			pushed += got;
			if (pushed == f) break;
		}
		return pushed;
	}
	ll maxflow(int s, int t){
		this->s = s; this->t = t;
		if (s == t) return 0;
		ll flow = 0;
		for (ll lim = oo; lim; lim >>= 1)
			while (bfs(lim)){
				fill(ptr.begin(), ptr.end(), 0);
				flow += dfs(s, oo, lim);
			}
		return flow;
	}
	ll cap(int id){ return g[at[id].fi][at[id].se].cap; }
};
