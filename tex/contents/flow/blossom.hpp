#pragma once
#include "../contest/template.hpp"
struct Blossom{
	int n, cnt = 0;
	vector<vector<int>> g;
	// state: 0 = unvisited, 1 = odd, 2 = even
	vector<int> mate, par, state, link, vis;
	Blossom(int n): n(n), g(n), mate(n + 1, n), par(n + 1),
		state(n + 1), link(n + 1, n), vis(n + 1){}
	void addEdge(int u, int v){
		g[u].push_back(v); g[v].push_back(u);
	}
	int find(int x){
		return x == par[x] ? x : par[x] = find(par[x]);
	}
	int lca(int u, int v){
		cnt++;
		for (; vis[u] != cnt; swap(u, v)){
			if (u != n){
				vis[u] = cnt;
				u = find(link[mate[u]]);
			}
		}
		return u;
	}
	void shrink(int u, int v, int rt, queue<int> &q){
		for (; find(u) != rt; u = link[v]){
			link[u] = v;
			v = mate[u];
			par[u] = rt; par[v] = rt;
			if (state[v] == 1){
				q.push(v); state[v] = 2;
			}
		}
	}
	bool augment(int s){
		iota(par.begin(), par.end(), 0);
		fill(state.begin(), state.end(), 0);
		queue<int> q({s}); state[s] = 2;
		while (!q.empty()){
			int u = q.front(); q.pop();
			for (int v: g[u]){
				if (!state[v]){
					link[v] = u; state[v] = 1;
					if (mate[v] == n){
						for (; u != n; v = u){
							int p = link[v], nxt = mate[p];
							mate[v] = p; mate[p] = v;
							u = nxt;
						}
						return true;
					}
					q.push(mate[v]); state[mate[v]] = 2;
				} else if (state[v] == 2){
					int l = lca(find(u), find(v));
					shrink(u, v, l, q); shrink(v, u, l, q);
				}
			}
		}
		return false;
	}
	int run(){
		int res = 0;
		FOR(int, i, 0, n - 1)
			if (mate[i] == n) res += augment(i);
		return res;
	}
};
