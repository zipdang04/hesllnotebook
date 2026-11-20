#pragma once
#include "../contest/template.hpp"

// AHU: exact class ids of rooted subtrees, vertices 1..n,
// undirected g. One object = one id table. O(n log n).
// Unrooted: min over the 1 or 2 centres.
struct AHU{
	// sorted child classes -> class id 1, 2, ..
	map<vector<int>, int> ids;
	vector<int> rooted(const vector<vector<int>> &g,
			int root){
		int n = sz(g) - 1;
		vector<int> par(n + 1), id(n + 1), ord = {root};
		for(int i = 0; i < sz(ord); i++){
			int u = ord[i];
			for(int v: g[u]) if(v != par[u])
				par[v] = u, ord.push_back(v);
		}
		vector<vector<int>> ch(n + 1);
		REV(i, sz(ord) - 1, 0){
			int u = ord[i];
			sort(all(ch[u]));
			// a new key gets the next id
			id[u] = ids.try_emplace(ch[u], sz(ids) + 1).fi->se;
			ch[par[u]].push_back(id[u]);
		}
		return id;
	}
	int unrooted(const vector<vector<int>> &g){
		int n = sz(g) - 1, best = INT_MAX;
		vector<int> deg(n + 1), layer(n + 1), q;
		FOR(u, 1, n){
			deg[u] = sz(g[u]); if(deg[u] <= 1) q.push_back(u);
		}
		// peel leaves; the last layer holds the centres
		for(int i = 0; i < sz(q); i++){
			int u = q[i];
			for(int v: g[u]){
				deg[v]--;
				if(deg[v] == 1)
					layer[v] = layer[u] + 1, q.push_back(v);
			}
		}
		for(int c: q) if(layer[c] == layer[q.back()])
			minimize(best, rooted(g, c)[c]);
		return best;
	}
};
