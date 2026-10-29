#pragma once
#include "../contest/template.hpp"

struct Blossom{
	int n, cnt = 0;
	vector<vector<int>> g;
	vector<int> mate, par, chk, prv, vis; // mate[v] == n: v is free
	Blossom(int n): n(n), g(n), mate(n + 1, n), par(n + 1), chk(n + 1), prv(n + 1, n), vis(n + 1) {}
	void addEdge(int u, int v){ g[u].push_back(v); g[v].push_back(u); }
	int find(int x){ return x == par[x] ? x : par[x] = find(par[x]); }
	int lca(int u, int v){
		for (cnt++; vis[u] != cnt; swap(u, v))
			if (u != n) vis[u] = cnt, u = find(prv[mate[u]]);
		return u;
	}
	void shrink(int u, int v, int rt, queue<int> &q){
		for (; find(u) != rt; u = prv[v]){
			prv[u] = v; par[u] = par[v = mate[u]] = rt;
			if (chk[v] == 1) q.push(v), chk[v] = 2;
		}
	}
	bool augment(int s){
		iota(par.begin(), par.end(), 0); fill(chk.begin(), chk.end(), 0);
		queue<int> q; q.push(s); chk[s] = 2;
		while (!q.empty()){
			int u = q.front(); q.pop();
			for (int v: g[u]){
				if (!chk[v]){
					prv[v] = u; chk[v] = 1;
					if (mate[v] == n){
						for (; u != n; v = u) u = mate[prv[v]], mate[mate[v] = prv[v]] = v;
						return true;
					}
					q.push(mate[v]); chk[mate[v]] = 2;
				} else if (chk[v] == 2){
					int l = lca(find(u), find(v)); shrink(u, v, l, q); shrink(v, u, l, q);
				}
			}
		}
		return false;
	}
	int run(){ // max matching size
		int res = 0;
		FOR(int, i, 0, n - 1) if (mate[i] == n) res += augment(i);
		return res;
	}
};
