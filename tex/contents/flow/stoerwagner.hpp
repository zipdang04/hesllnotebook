#pragma once
#include "../contest/template.hpp"

// Global min cut (Stoer-Wagner), undirected, vertices 1..n.
// O(E + V^3). No recursion.
using i128 = __int128;
pair<ll, vector<int>> globalMinCut(
		const vector<vector<pair<int, ll>>> &g){
	int n = sz(g) - 1;
	// Sums may exceed ll; 2^64 + oo casts to oo (n = 1).
	vector<vector<i128>> mat(n + 1, vector<i128>(n + 1));
	FOR(u, 1, n) for(auto [v, w]: g[u]) if(u != v)
		mat[u][v] += w;
	pair<i128, vector<int>> best = {(i128(1) << 64) + oo, {}};
	vector<vector<int>> co(n + 1);
	FOR(i, 1, n)
		co[i] = {i};
	vector<char> merged(n + 1);
	for(int ph = 1; ph < n && best.fi; ph++){
		vector<i128> w = mat[1]; int s = 1, t = 1;
		vector<char> used = merged;
		FOR(it, 1, n - ph){
			used[t] = 1; s = t, t = 0;
			FOR(i, 1, n) if(!used[i] && (!t || w[i] > w[t]))
				t = i;
			FOR(i, 1, n)
				w[i] += mat[t][i];
		}
		best = min(best, {w[t] - mat[t][t], co[t]});
		co[s].insert(co[s].end(), all(co[t]));
		FOR(i, 1, n)
			mat[i][s] = mat[s][i] += mat[t][i];
		merged[t] = 1;
	}
	return {(ll)best.fi, best.se};
}
