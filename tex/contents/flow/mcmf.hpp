#pragma once
#include "../contest/template.hpp"

// Min-cost flow: successive shortest paths, Dijkstra with
// potentials, Bellman-Ford once per run. MCMF mf(N):
// vertices 0..N-1. O(VE + F E log V). No recursion.
struct MCMF{
	struct Edge{ int to; ll cap, cost; };
	vector<Edge> edges;
	vector<vector<int>> adj;
	vector<ll> pot, dist;
	vector<int> par;
	MCMF(int n_): adj(n_), pot(n_), dist(n_), par(n_){}
	void setPot(){
		fill(all(pot), 0);
		FOR(i, 0, sz(adj) - 1){
			bool changed = false;
			FOR(u, 0, sz(adj) - 1)
				for(int id: adj[u]){
					auto &e = edges[id];
					if(e.cap && minimize(pot[e.to], pot[u] + e.cost))
						changed = true;
				}
			if(!changed)
				break;
		}
	}
	bool dijkstra(int s, int t){
		// Costs may exceed oo.
		fill(all(dist), LLONG_MAX); dist[s] = 0;
		PQMin<pll> pq; pq.push({0, s});
		while(!pq.empty()){
			auto [d, u] = pq.top(); pq.pop();
			if(d != dist[u])
				continue;
			for(int id: adj[u]){
				auto &e = edges[id];
				if(!e.cap)
					continue;
				ll val = d + e.cost + pot[u] - pot[e.to];
				if(minimize(dist[e.to], val)){
					par[e.to] = id;
					pq.push({val, e.to});
				}
			}
		}
		return dist[t] != LLONG_MAX;
	}
	// Returns the arc id (0, 2, 4, ..).
	int addEdge(int u, int v, ll cap, ll cost){
		int id = sz(edges);
		edges.push_back({v, cap, cost});
		edges.push_back({u, 0, -cost});
		adj[u].push_back(id); adj[v].push_back(id ^ 1);
		return id;
	}
	pair<ll, ll> maxFlow(int s, int t, ll limit = oo){
		if(s == t) return {0, 0};
		ll flow = 0, cost = 0;
		setPot();
		while(flow < limit && dijkstra(s, t)){
			// Unreached stay so; never add LLONG_MAX.
			FOR(v, 0, sz(adj) - 1) if(dist[v] != LLONG_MAX)
				pot[v] += dist[v];
			ll push = limit - flow;
			for(int v = t; v != s; v = edges[par[v] ^ 1].to)
				minimize(push, edges[par[v]].cap);
			for(int v = t; v != s; v = edges[par[v] ^ 1].to){
				edges[par[v]].cap -= push;
				edges[par[v] ^ 1].cap += push;
			}
			flow += push; cost += push * (pot[t] - pot[s]);
		}
		return {flow, cost};
	}
	ll flowOf(int id){ return edges[id ^ 1].cap; }
};
