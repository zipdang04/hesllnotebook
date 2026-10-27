#pragma once
#include "dinitzwscaling.hpp"

// Min-cost circulation with bounds l <= f <= r and any costs; cost-scaling push-relabel (koosaga).
// O(V^2 E log(VC)), independent of capacities. After solve(): flowOf(i), p = duals.
struct MinCostCirculation {
	struct In { int s, t; ll l, r, c; };
	struct Edge { int to, rev; ll rem, c; };
	int n; vector<In> in; vector<vector<Edge>> g; vector<int> pos; vector<ll> p;
	MinCostCirculation(int n): n(n), g(n), p(n) {}
	int addEdge(int s, int t, ll l, ll r, ll c) { in.push_back({s, t, l, r, c}); return in.size() - 1; }
	ll flowOf(int i) {
		auto &e = in[i];
		if (e.s == e.t) return e.c < 0 ? e.r : e.l;            // self-loops never enter the graph
		return e.r - g[e.s][pos[i]].rem;
	}

	pair<bool, __int128> solve() {
		g.assign(n, {}); p.assign(n, 0);
		// 1. feasible circulation: lower-bound reduction + max flow
		Flow f; f.init(n + 2, n, n + 1);
		vector<ll> ex(n); ll need = 0;
		for (auto &e: in) if (e.s != e.t) f.addEdge(e.s, e.t, e.r - e.l), ex[e.t] += e.l, ex[e.s] -= e.l;
		for (int v = 0; v < n; v++)
			if (ex[v] > 0) f.addEdge(n, v, ex[v]), need += ex[v];
			else if (ex[v] < 0) f.addEdge(v, n + 1, -ex[v]);
		if (f.calculate() != need) return {false, 0};
		// 2. residual graph, costs scaled by n+1 so that eps <= 1 means optimal
		pos.assign(in.size(), -1);
		for (int i = 0, k = 0; i < (int)in.size(); i++) {
			auto &e = in[i]; if (e.s == e.t) continue;
			ll cur = e.r - f.capOf(k++);                      // flow on e, in [l, r]
			pos[i] = g[e.s].size();
			g[e.s].push_back({e.t, (int)g[e.t].size(), e.r - cur, e.c * (n + 1)});
			g[e.t].push_back({e.s, pos[i], cur - e.l, -e.c * (n + 1)});
		}
		// 3. refine: eps /= 8 per phase; saturate negative edges, then push-relabel the excess away
		auto rc = [&](Edge &e, int u) { return e.c + p[u] - p[e.to]; };
		for (ll eps; ; ) {
			eps = 0;
			for (int u = 0; u < n; u++) for (auto &e: g[u]) if (e.rem > 0) eps = max(eps, -rc(e, u));
			if (eps <= 1) break;
			eps = max(1ll, eps >> 3);
			vector<ll> excess(n); vector<int> ptr(n); queue<int> q;
			auto push = [&](Edge &e, int u, ll x) {
				e.rem -= x; g[e.to][e.rev].rem += x; excess[u] -= x; excess[e.to] += x;
				if (excess[e.to] > 0 && excess[e.to] <= x) q.push(e.to);   // just became active
			};
			for (int u = 0; u < n; u++) for (auto &e: g[u]) if (e.rem > 0 && rc(e, u) < 0) push(e, u, e.rem);
			while (!q.empty()) {
				int u = q.front(); q.pop();
				while (excess[u] > 0) {
					for (; ptr[u] < (int)g[u].size() && excess[u] > 0; ptr[u]++) {
						Edge &e = g[u][ptr[u]];
						if (e.rem > 0 && rc(e, u) < 0) push(e, u, min(e.rem, excess[u]));
					}
					if (excess[u] > 0) {                      // relabel: cheapest residual edge gets rc = -eps
						ptr[u] = 0; p[u] = LLONG_MIN / 4;
						for (auto &e: g[u]) if (e.rem > 0) p[u] = max(p[u], p[e.to] - e.c - eps);
					}
				}
			}
		}
		// 4. duals (Bellman-Ford on the residual graph, real costs) and the answer
		for (auto &v: g) for (auto &e: v) e.c /= n + 1;
		fill(p.begin(), p.end(), 0);
		for (bool upd = true; upd; ) { upd = false;
			for (int u = 0; u < n; u++) for (auto &e: g[u])
				if (e.rem > 0 && p[u] + e.c < p[e.to]) p[e.to] = p[u] + e.c, upd = true; }
		__int128 ans = 0;
		for (int i = 0; i < (int)in.size(); i++) ans += (__int128)in[i].c * flowOf(i);
		return {true, ans};
	}
};
