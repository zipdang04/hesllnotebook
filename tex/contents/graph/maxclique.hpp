#pragma once
#include "../contest/template.hpp"

// Maximum clique, branch and bound with greedy colouring.
// Vertices 1..n, n < 128 (edit B). Exponential: about
// 1 s at n = 150 dense. Recursive, depth <= clique + 1.
struct MaxClique{
	// ---- EDIT: bitset size > n ----
	typedef bitset<128> B;
	// ---- end of EDIT ----
	struct Vertex{ int i, d = 0; };
	vector<int> clique;
	vector<int> cur, steps, old;
	vector<B> adj;
	vector<vector<int>> col;
	// re-colour when few nodes were expanded
	double limit = 0.025, pk = 0;
	MaxClique(const vector<vector<int>> &g):
			steps(sz(g) + 1), old(sz(g) + 1), adj(sz(g)),
			col(sz(g) + 1){
		int n = sz(g) - 1;
		vector<Vertex> vs;
		FOR(u, 1, n){
			vs.push_back({u});
			for(int v: g[u]) if(v != u)
				adj[u][v] = 1;
		}
		if(n) initDeg(vs);
		expand(vs);
	}
	void initDeg(vector<Vertex> &r){
		for(auto &v: r) v.d = 0;
		for(auto &v: r) for(auto j: r)
			v.d += adj[v.i][j.i];
		sort(all(r), [](const Vertex &a, const Vertex &b){
			return a.d > b.d;
		});
		int mxD = r[0].d;
		FOR(i, 0, sz(r) - 1) r[i].d = min(i, mxD) + 1;
	}
	void expand(vector<Vertex> &R, int lev = 1){
		steps[lev] += steps[lev-1] - old[lev];
		old[lev] = steps[lev-1];
		while(sz(R)){
			if(sz(cur) + R.back().d <= sz(clique)) return;
			cur.push_back(R.back().i);
			vector<Vertex> T;
			for(auto v: R) if(adj[R.back().i][v.i])
				T.push_back({v.i});
			if(sz(T)){
				pk++;
				if(steps[lev] / pk < limit) initDeg(T);
				steps[lev]++;
				int j = 0, mxk = 1;
				int mnk = max(sz(clique) - sz(cur) + 1, 1);
				col[1].clear(); col[2].clear();
				for(auto v: T){
					int k = 1;
					auto f = [&](int i){ return adj[v.i][i]; };
					while(any_of(all(col[k]), f)) k++;
					if(k > mxk) mxk = k, col[mxk+1].clear();
					if(k < mnk) T[j++].i = v.i;
					col[k].push_back(v.i);
				}
				if(j > 0) T[j-1].d = 0;
				FOR(k, mnk, mxk) for(int i: col[k]){
					T[j].i = i; T[j++].d = k;
				}
				expand(T, lev+1);
			}else if(sz(cur) > sz(clique)) clique = cur;
			cur.pop_back(); R.pop_back();
		}
	}
};
