#pragma once
#include "../contest/template.hpp"
template<class C = ll> struct MCMF{
	struct Edge{ int to; ll cap, cost; };
	static constexpr ll oo = 1ll << 62;
	int n;
	vector<Edge> e, resi;
	vector<vector<int>> g;
	MCMF(int n): n(n), g(n){}
	int addEdge(int u, int v, ll cap, ll cost){
		g[u].push_back(e.size()); e.push_back({v, cap, cost});
		g[v].push_back(e.size()); e.push_back({u, 0, -cost});
		return (int)e.size() / 2 - 1;
	}
	pair<ll, C> run(int s, int t, ll limit = oo){
		resi = e;
		ll flow = 0; C cost = 0;
		vector<ll> dist(n);
		vector<int> par(n);
		vector<char> inq(n);
		while (s != t && flow < limit){
			fill(dist.begin(), dist.end(), oo);
			dist[s] = 0;
			queue<int> q;
			q.push(s); inq[s] = 1;
			while (!q.empty()){
				int u = q.front(); q.pop(); inq[u] = 0;
				for (int id: g[u]){
					Edge &x = resi[id];
					ll d = dist[u] + x.cost;
					if (!x.cap || d >= dist[x.to]) continue;
					dist[x.to] = d; par[x.to] = id;
					if (!inq[x.to]){ inq[x.to] = 1; q.push(x.to); }
				}
			}
			if (dist[t] == oo) break;
			ll push = limit - flow;
			for (int v = t; v != s; v = resi[par[v] ^ 1].to)
				push = min(push, resi[par[v]].cap);
			for (int v = t; v != s; v = resi[par[v] ^ 1].to){
				resi[par[v]].cap -= push;
				resi[par[v] ^ 1].cap += push;
			}
			flow += push; cost += (C)push * dist[t];
		}
		return {flow, cost};
	}
	ll flow(int id){ return resi[2 * id + 1].cap; }
};
