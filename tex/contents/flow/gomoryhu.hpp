#pragma once
#include "dinitzwscaling.hpp"
struct GomoryHu{
	int n;
	Dinic base;
	vector<int> par;
	vector<ll> cut;
	GomoryHu(int n): n(n), base(n), par(n), cut(n){}
	void addEdge(int u, int v, ll w){
		base.addEdge(u, v, w); base.addEdge(v, u, w);
	}
	void run(){
		FOR(int, i, 1, n - 1){
			Dinic f = base;
			cut[i] = f.maxflow(i, par[i]);
			// j is on i's side of the cut
			FOR(int, j, i + 1, n - 1)
				if (par[j] == par[i] && f.level[j] < 0)
					par[j] = i;
		}
	}
};
