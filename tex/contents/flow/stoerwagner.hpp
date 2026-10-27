#pragma once
#include "../contest/template.hpp"

struct StoerWagner {
	int n; vector<vector<ll>> g;
	StoerWagner(int n): n(n), g(n, vector<ll>(n)) {}
	void addEdge(int u, int v, ll w) { g[u][v] += w; g[v][u] += w; }
	pair<ll, vector<int>> run() {
		pair<ll, vector<int>> best = {n > 1 ? LLONG_MAX : 0, {}};
		vector<vector<int>> co(n); vector<char> dead(n);   // co[v] = original vertices merged into v
		EFOR(int, i, 0, n) co[i] = {i};
		for (int ph = n - 1; ph > 0; ph--) {
			vector<ll> w(n); vector<char> in = dead; int s = -1, t = -1;
			FOR(int, it, 0, ph) {                            // maximum adjacency order
				s = t; t = -1;
				EFOR(int, j, 0, n) if (!in[j] && (t < 0 || w[j] > w[t])) t = j;
				in[t] = 1;
				EFOR(int, j, 0, n) if (!in[j]) w[j] += g[t][j];
			}
			if (w[t] < best.fi) best = {w[t], co[t]};         // cut of the phase: {t} vs rest
			co[s].insert(co[s].end(), co[t].begin(), co[t].end()); dead[t] = 1;
			EFOR(int, j, 0, n) g[s][j] += g[t][j], g[j][s] = g[s][j];   // merge t into s
		}
		return best;
	}
};
