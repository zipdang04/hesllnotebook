#pragma once
#include "../contest/template.hpp"
#include "dinitzwscaling.hpp"

struct GomoryHu{
	int n; Flow base; vector<int> par; vector<ll> cut;
	GomoryHu(int n): n(n), par(n), cut(n){ base.init(n, 0, 0); }
	void addEdge(int u, int v, ll w){ base.addEdge(u, v, w); base.addEdge(v, u, w); }
	void run(){ // tree edges: (i, par[i]) of weight cut[i], i >= 1
		FOR(int, i, 1, n - 1){
			Flow f = base; f.s = i; f.t = par[i]; cut[i] = f.calculate();
			FOR(int, j, i + 1, n - 1) if (par[j] == par[i] && f.level[j] < 0) par[j] = i; // j on i's side
		}
	}
};
