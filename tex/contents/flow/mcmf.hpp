#pragma once
#include "../graph/spfa.hpp"

// successive shortest paths (SPFA on the residual graph); no negative cycles. C = type of the total cost
template<class C = ll> struct MCMF{
	struct Edge{int from, dest, oppo; ll capa, cost;};
	vector<Edge> edges, resi;
	vector<vector<int>> graph;
	SPFA sp;
	int n, s, t;

	void init(int n, int s, int t){
		this -> n = n; this -> s = s; this -> t = t;
		edges.clear(); graph.assign(n, {});
	}

	int addEdge(int u, int v, ll capa, ll cost){ // returns edge id
		int m = edges.size();
		edges.push_back({u, v, m + 1, capa, cost});
		edges.push_back({v, u, m, 0, -cost});
		graph[u].push_back(m); graph[v].push_back(m + 1);
		return m / 2;
	}

	pair<ll, C> calculate(ll limit = 1ll << 62){ // {flow, cost}, flow <= limit
		resi = edges; ll flow = 0; C cost = 0;
		if (s == t) return {0, 0};
		auto residual = [&](int u, auto relax){
			for (int idx: graph[u]) if (resi[idx].capa > 0) relax(resi[idx].dest, resi[idx].cost, idx);
		};
		while (flow < limit){
			sp.run(n, s, residual);
			if (sp.dist[t] >= SPFA::oo) break;
			ll push = limit - flow;
			for (int v = t; v != s; v = resi[sp.par[v]].from) push = min(push, resi[sp.par[v]].capa);
			for (int v = t; v != s; v = resi[sp.par[v]].from){
				int e = sp.par[v];
				resi[e].capa -= push; resi[resi[e].oppo].capa += push;
			}
			flow += push; cost += (C)push * sp.dist[t];
		}
		return {flow, cost};
	}

	ll flowOn(int id){ return resi[2 * id + 1].capa; }
};
