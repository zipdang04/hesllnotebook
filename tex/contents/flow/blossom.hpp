#pragma once
#include "../contest/template.hpp"

// Maximum matching in a general graph (Edmonds, BFS),
// vertices 1..n, O(V^3). Recursive find, depth <= V.
struct GeneralMatching{
	int pairs = 0, stamp = 0;
	// chk: 0 unseen, 1 odd, 2 even (in the queue)
	vector<int> match, par, chk, prv, vis;
	GeneralMatching(const vector<vector<int>> &g):
			match(sz(g)), par(sz(g)), chk(sz(g)), prv(sz(g)),
			vis(sz(g)){
		FOR(u, 1, sz(g) - 1) if(!match[u])
			pairs += augment(u, g);
	}
	int find(int x){
		if(par[x] != x) par[x] = find(par[x]);
		return par[x];
	}
	int lca(int u, int v){
		for(stamp++; vis[u] != stamp; swap(u, v)) if(u){
			vis[u] = stamp;
			u = find(prv[match[u]]);
		}
		return u;
	}
	void blossom(int u, int v, int rt, queue<int> &q){
		for(; find(u) != rt; u = prv[v]){
			prv[u] = v;
			v = match[u];
			par[u] = par[v] = rt;
			if(chk[v] == 1) q.push(v), chk[v] = 2;
		}
	}
	bool augment(int u, const vector<vector<int>> &g){
		iota(all(par), 0); fill(all(chk), 0);
		queue<int> q;
		q.push(u); chk[u] = 2;
		while(!q.empty()){
			u = q.front(); q.pop();
			for(int v: g[u]){
				// a self-loop: lca(u, u) may not be a blossom base
				if(v == u) continue;
				if(chk[v] == 2){
					int l = lca(u, v);
					blossom(u, v, l, q), blossom(v, u, l, q);
					continue;
				}
				if(chk[v]) continue;
				prv[v] = u; chk[v] = 1;
				q.push(match[v]); chk[match[v]] = 2;
				if(match[v]) continue;
				for(; u; v = u){
					u = match[prv[v]];
					match[v] = prv[v];
					match[prv[v]] = v;
				}
				return true;
			}
		}
		return false;
	}
};
