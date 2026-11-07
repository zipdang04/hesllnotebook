#pragma once
#include "dinitzwscaling.hpp"
struct MinCostCirculation{
	struct In{ int u, v; ll l, r, c; };
	struct Edge{ int to, rev; ll rem, c; };
	int n;
	vector<In> in;
	vector<vector<Edge>> g;
	vector<int> pos;
	vector<ll> p;
	MinCostCirculation(int n): n(n), g(n), p(n){}
	int addEdge(int u, int v, ll l, ll r, ll c){
		in.push_back({u, v, l, r, c});
		return (int)in.size() - 1;
	}
	ll flow(int i){
		auto &e = in[i];
		// self-loops never enter the graph
		if (e.u == e.v) return e.c < 0 ? e.r : e.l;
		return e.r - g[e.u][pos[i]].rem;
	}
	pair<bool, __int128> run(){
		g.assign(n, {}); p.assign(n, 0);
		// 1. feasible circulation: lower bounds + max flow
		Dinic f(n + 2);
		vector<ll> ex(n); ll need = 0;
		for (auto &e: in)
			if (e.u != e.v){
				f.addEdge(e.u, e.v, e.r - e.l);
				ex[e.v] += e.l; ex[e.u] -= e.l;
			}
		FOR(int, v, 0, n - 1){
			if (ex[v] > 0){
				f.addEdge(n, v, ex[v]);
				need += ex[v];
			} else if (ex[v] < 0) f.addEdge(v, n + 1, -ex[v]);
		}
		if (f.maxflow(n, n + 1) != need) return {false, 0};
		// 2. residual graph, scale costs by n+1
		pos.assign(in.size(), -1);
		int k = 0;
		FOR(int, i, 0, (int)in.size() - 1){
			auto &e = in[i];
			if (e.u == e.v) continue;
			ll cur = e.r - f.cap(k++);
			pos[i] = g[e.u].size();
			g[e.u].push_back({e.v, (int)g[e.v].size(),
				e.r - cur, e.c * (n + 1)});
			g[e.v].push_back({e.u, pos[i], cur - e.l,
				-e.c * (n + 1)});
		}
		// 3. refine: saturate negative edges, push-relabel
		auto rc = [&](Edge &e, int u){
			return e.c + p[u] - p[e.to];
		};
		for (ll eps;;){
			eps = 0;
			FOR(int, u, 0, n - 1)
				for (Edge &e: g[u])
					if (e.rem > 0) eps = max(eps, -rc(e, u));
			if (eps <= 1) break;
			eps = max(1ll, eps >> 3);
			vector<ll> excess(n);
			vector<int> ptr(n);
			queue<int> q;
			auto push = [&](Edge &e, int u, ll x){
				e.rem -= x; g[e.to][e.rev].rem += x;
				excess[u] -= x; excess[e.to] += x;
				if (excess[e.to] > 0 && excess[e.to] <= x)
					q.push(e.to);
			};
			FOR(int, u, 0, n - 1)
				for (Edge &e: g[u])
					if (e.rem > 0 && rc(e, u) < 0)
						push(e, u, e.rem);
			while (!q.empty()){
				int u = q.front(); q.pop();
				while (excess[u] > 0){
					for (; ptr[u] < (int)g[u].size() &&
						excess[u] > 0; ptr[u]++){
						Edge &e = g[u][ptr[u]];
						if (e.rem > 0 && rc(e, u) < 0)
							push(e, u, min(e.rem, excess[u]));
					}
					if (excess[u] > 0){
						// relabel: set rc = -eps on best edge
						ptr[u] = 0; p[u] = LLONG_MIN / 4;
						for (Edge &e: g[u])
							if (e.rem > 0)
								p[u] = max(p[u], p[e.to] - e.c - eps);
					}
				}
			}
		}
		// 4. duals via Bellman-Ford, and answer
		for (auto &v: g)
			for (Edge &e: v) e.c /= n + 1;
		fill(p.begin(), p.end(), 0);
		for (bool upd = true; upd;){
			upd = false;
			FOR(int, u, 0, n - 1)
				for (Edge &e: g[u])
					if (e.rem > 0 && p[u] + e.c < p[e.to]){
						p[e.to] = p[u] + e.c;
						upd = true;
					}
		}
		__int128 ans = 0;
		FOR(int, i, 0, (int)in.size() - 1)
			ans += (__int128)in[i].c * flow(i);
		return {true, ans};
	}
};
