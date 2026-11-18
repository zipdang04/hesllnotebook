#pragma once
#include "../contest/template.hpp"

// Global min cut (Stoer-Wagner), undirected, vertices 1..n.
// O(V^3). No recursion.
pair<ll, vector<int>> globalMinCut(
		const vector<vector<pair<int, ll>>> &g){
	int n = sz(g) - 1;
	// Intermediate sums may exceed ll even if the cut fits.
	vector<vector<__int128>> mat(n + 1,
		vector<__int128>(n + 1));
	FOR(u, 1, n) for(auto [v, w]: g[u]) if(u != v)
		mat[u][v] += w;
	pair<__int128, vector<int>> best = {oo, {}};
	vector<vector<int>> co(n + 1);
	FOR(i, 1, n)
		co[i] = {i};
	vector<char> merged(n + 1);
	FOR(ph, 1, n - 1){
		vector<__int128> w = mat[1]; int s = 1, t = 1;
		vector<char> used = merged;
		FOR(it, 1, n - ph){
			used[t] = 1; s = t, t = 0;
			FOR(i, 1, n) if(!used[i] && (!t || w[i] > w[t]))
				t = i;
			FOR(i, 1, n)
				w[i] += mat[t][i];
		}
		if(ph == 1) best = {w[t] - mat[t][t], co[t]};
		best = min(best, {w[t] - mat[t][t], co[t]});
		co[s].insert(co[s].end(), all(co[t]));
		FOR(i, 1, n)
			mat[s][i] += mat[t][i];
		FOR(i, 1, n)
			mat[i][s] = mat[s][i];
		merged[t] = 1;
	}
	return {(ll)best.fi, best.se};
}
