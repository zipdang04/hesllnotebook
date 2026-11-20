#pragma once
#include "dinic.hpp"

// Min-cost circulation, lo <= f <= hi, any costs: cost
// scaling push-relabel, O(V^2 E log(VC)). mc(N): vertices
// 0..N-1. Feasibility by Dinic: recursive, depth <= V + 2.
struct MinCostCirc{
	struct Edge{ int to; ll cap, cost; };
	int n;
	vector<Edge> edges; // edges[id ^ 1] = reverse of id
	vector<vector<int>> adj; // ids of arcs out of u
	vector<ll> low, pot; // low[id / 2]; pot = duals
	__int128 total = 0; // min cost; exceeds ll
	MinCostCirc(int n_): n(n_), adj(n_), pot(n_){}
	// A self-loop stays out of adj. Returns the arc id.
	int addEdge(int u, int v, ll lo, ll hi, ll cost){
		int id = sz(edges);
		edges.push_back({v, hi - lo, cost});
		edges.push_back({u, 0, -cost}), low.push_back(lo);
		if(u != v)
			adj[u].push_back(id), adj[v].push_back(id + 1);
		return id;
	}
	ll reducedCost(int u, int id){
		return edges[id].cost + pot[u] - pot[edges[id].to];
	}
	// Refine by eps / 8; false means already optimal.
	bool refine(){
		ll eps = 0;
		FOR(u, 0, n - 1) for(int id: adj[u])
			if(edges[id].cap > 0)
				maximize(eps, -reducedCost(u, id));
		if(eps <= 1) return false;
		eps = max(1LL, eps >> 3);
		vector<ll> excess(n); vector<int> ptr(n); queue<int> q;
		auto push = [&](int u, int id, ll x){
			int v = edges[id].to;
			edges[id].cap -= x, edges[id ^ 1].cap += x;
			excess[u] -= x, excess[v] += x;
			if(excess[v] > 0 && excess[v] <= x) q.push(v);
		};
		FOR(u, 0, n - 1) for(int id: adj[u])
			if(edges[id].cap > 0 && reducedCost(u, id) < 0)
				push(u, id, edges[id].cap);
		while(!q.empty()){
			int u = q.front(); q.pop();
			while(excess[u] > 0){
				for(; ptr[u] < sz(adj[u]) && excess[u]; ptr[u]++){
					int id = adj[u][ptr[u]];
					if(edges[id].cap > 0 && reducedCost(u, id) < 0)
						push(u, id, min(edges[id].cap, excess[u]));
				}
				if(excess[u] == 0) break;
				// Excess: a reverse arc has room.
				ptr[u] = 0, pot[u] = LLONG_MIN;
				for(int id: adj[u]) if(edges[id].cap > 0)
					maximize(pot[u],
						pot[edges[id].to] - edges[id].cost - eps);
			}
		}
		return true;
	}
	bool solve(){
		Dinic f(n + 2); vector<ll> ex(n); ll need = 0;
		for(int id = 0; id < sz(edges); id += 2){
			int u = edges[id ^ 1].to, v = edges[id].to;
			f.addEdge(u, v, edges[id].cap);
			ex[v] += low[id / 2], ex[u] -= low[id / 2];
		}
		FOR(v, 0, n - 1)
			if(ex[v] > 0) f.addEdge(n, v, ex[v]), need += ex[v];
			else if(ex[v] < 0) f.addEdge(v, n + 1, -ex[v]);
		if(need && f.maxFlow(n, n + 1) < need) return false;
		FOR(id, 0, sz(edges) - 1)
			edges[id].cap = f.edges[id].cap;
		for(int id = 0; id < sz(edges); id += 2){
			Edge &e = edges[id], &r = edges[id ^ 1];
			if(e.to == r.to && e.cost < 0) swap(e.cap, r.cap);
		}
		// costs * (n + 1): eps <= 1 is then optimal
		for(Edge &e: edges) e.cost *= n + 1;
		while(refine());
		for(Edge &e: edges) e.cost /= n + 1;
		fill(all(pot), 0);
		for(bool upd = true; upd; ){
			upd = false;
			FOR(u, 0, n - 1) for(int id: adj[u]){
				Edge &e = edges[id];
				if(e.cap > 0 && pot[u] + e.cost < pot[e.to])
					pot[e.to] = pot[u] + e.cost, upd = true;
			}
		}
		for(int id = 0; id < sz(edges); id += 2)
			total += (__int128)edges[id].cost * flowOf(id);
		return true;
	}
	ll flowOf(int id){
		return low[id / 2] + edges[id ^ 1].cap;
	}
};
