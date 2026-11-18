#pragma once
#include "scc.hpp"

// 2-SAT via SCC. Variables 1..n, literal -x = not x.
// O(n + clauses). Recursive (SCC), depth <= 2n + 1.
struct TwoSat{
	int n;
	vector<vector<int>> g;
	vector<bool> val;
	TwoSat(int n_): n(n_), g(2 * n_ + 2){}
	int node(int x){ return x > 0 ? 2 * x : -2 * x + 1; }
	void either(int a, int b){
		g[node(-a)].push_back(node(b));
		g[node(-b)].push_back(node(a));
	}
	bool solve(){
		val.assign(n+1, false); SCC scc(g);
		FOR(x, 1, n){
			if(scc.comp[node(x)] == scc.comp[node(-x)])
				return false;
			val[x] = scc.comp[node(x)] < scc.comp[node(-x)];
		}
		return true;
	}
};
