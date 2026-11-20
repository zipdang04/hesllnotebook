#pragma once
#include "../contest/template.hpp"

// DSU with rollback. Elements 0..n-1: RollbackDsu(n + 1)
// for 1..n. Union by size, no path compression, so find
// is O(log n) and rollback undoes joins.
struct RollbackDsu{
	vector<int> par;
	vector<pair<int, int>> stk;
	RollbackDsu(int n): par(n, -1){}
	int find(int u){
		while(par[u] >= 0) u = par[u];
		return u;
	}
	int size(int u){ return -par[find(u)]; }
	bool unite(int u, int v){
		u = find(u), v = find(v);
		if(u == v) return false;
		if(par[u] > par[v]) swap(u, v);
		stk.push_back({v, par[v]});
		par[u] += par[v], par[v] = u;
		return true;
	}
	int time(){ return sz(stk); }
	void rollback(int t){
		while(sz(stk) > t){
			auto [v, old] = stk.back(); stk.pop_back();
			par[par[v]] -= old, par[v] = old;
		}
	}
};
