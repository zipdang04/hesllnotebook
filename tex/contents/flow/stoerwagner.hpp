#pragma once
#include "../contest/template.hpp"
struct StoerWagner{
	int n;
	vector<vector<ll>> g;
	StoerWagner(int n): n(n), g(n, vector<ll>(n)){}
	void addEdge(int u, int v, ll w){
		g[u][v] += w; g[v][u] += w;
	}
	pair<ll, vector<int>> run(){
		pair<ll, vector<int>> best{n > 1 ? LLONG_MAX : 0, {}};
		vector<vector<int>> part(n);
		vector<char> merged(n);
		FOR(int, i, 0, n - 1) part[i] = {i};
		REV(int, ph, n - 1, 1){
			vector<ll> w(n);
			vector<char> in = merged;
			int s = -1, t = -1;
			// maximum adjacency order
			FOR(int, it, 0, ph){
				s = t; t = -1;
				FOR(int, j, 0, n - 1)
					if (!in[j] && (t < 0 || w[j] > w[t])) t = j;
				in[t] = 1;
				FOR(int, j, 0, n - 1) if (!in[j]) w[j] += g[t][j];
			}
			// cut of the phase: {t} vs rest
			if (w[t] < best.fi) best = {w[t], part[t]};
			part[s].insert(part[s].end(),
				part[t].begin(), part[t].end());
			merged[t] = 1;
			// merge t into s
			FOR(int, j, 0, n - 1){
				g[s][j] += g[t][j]; g[j][s] = g[s][j];
			}
		}
		return best;
	}
};
