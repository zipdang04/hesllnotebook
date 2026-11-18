#include "../../tex/contents/flow/gomoryhu.hpp"
#include <cassert>

vector<ll> brute(
		const vector<vector<pair<int, ll>>> &g){
	int n = sz(g) - 1;
	vector<ll> cut(1 << n);
	FOR(mask, 0, (1 << n) - 1)
		FOR(u, 1, n) if(testBit(mask, u - 1))
			for(auto [v, w]: g[u])
				if(!testBit(mask, v - 1)) cut[mask] += w;
	return cut;
}

int main(){
	mt19937_64 rng(712367);
	FOR(it, 1, 2000){
		int n = 1 + (int)(rng() % 12);
		int m = (int)(rng() % 21);
		vector<vector<pair<int, ll>>> g(n + 1);
		FOR(i, 1, m){
			int u = 1 + (int)(rng() % n);
			int v = 1 + (int)(rng() % n);
			ll w = rng() % 11;
			g[u].push_back({v, w});
			g[v].push_back({u, w});
		}
		auto tree = gomoryHu(g);
		assert(sz(tree) == n - 1);
		vector<int> par(n + 1);
		FOR(u, 1, n) par[u] = u;
		vector<vector<pair<int, ll>>> adj(n + 1);
		for(auto e: tree){
			assert(1 <= e[0] && e[0] <= n);
			assert(1 <= e[1] && e[1] <= n);
			assert(e[2] >= 0);
			int u = (int)e[0], v = (int)e[1];
			int a = u, b = v;
			while(par[a] != a) a = par[a];
			while(par[b] != b) b = par[b];
			assert(a != b);
			par[a] = b;
			adj[u].push_back({v, e[2]});
			adj[v].push_back({u, e[2]});
		}
		auto cut = brute(g);
		FOR(x, 1, n){
			vector<ll> least(n + 1, -1);
			vector<int> q = {x};
			least[x] = oo;
			for(int i = 0; i < sz(q); i++){
				int u = q[i];
				for(auto [v, w]: adj[u]) if(least[v] < 0){
					least[v] = min(least[u], w);
					q.push_back(v);
				}
			}
			FOR(y, x + 1, n){
				ll expected = oo;
				FOR(mask, 0, sz(cut) - 1)
					if(testBit(mask, x - 1)
						&& !testBit(mask, y - 1))
						expected = min(expected, cut[mask]);
				assert(least[y] == expected);
				Dinic d(n + 1);
				FOR(u, 1, n)
					for(auto [v, w]: g[u]) d.addEdge(u, v, w);
				assert(d.maxFlow(x, y) == expected);
			}
		}
	}
	return 0;
}
