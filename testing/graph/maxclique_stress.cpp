#include "../../tex/contents/graph/maxclique.hpp"
#include <cassert>

int main(){
	mt19937 rng(12345);
	FOR(test, 1, 3000){
		int n = rng() % 15, pct = rng() % 101;
		vector<vector<int>> g(n + 1);
		vector<int> mat(n + 1);
		FOR(u, 1, n) FOR(v, u, n){
			if((int)(rng() % 100) >= pct) continue;
			int times = rng() % 4 ? 1 : 2;
			FOR(t, 1, times){
				g[u].push_back(v);
				if(u != v || rng() % 2) g[v].push_back(u);
			}
			if(u != v) mat[u] |= 1 << v, mat[v] |= 1 << u;
		}
		g[0].push_back(1);
		int best = 0;
		FOR(mask, 0, (1 << (n + 1)) - 1){
			if(mask & 1) continue;
			bool ok = true;
			FOR(u, 1, n) if(mask >> u & 1){
				int rest = mask & ~(1 << u);
				if((mat[u] & rest) != rest) ok = false;
			}
			if(ok) best = max(best, __builtin_popcount(mask));
		}
		MaxClique mc(g);
		auto &c = mc.clique;
		assert(sz(c) == best);
		FOR(i, 0, sz(c) - 1){
			assert(1 <= c[i] && c[i] <= n);
			FOR(j, 0, i - 1){
				assert(c[i] != c[j]);
				assert(mat[c[i]] >> c[j] & 1);
			}
		}
	}
	FOR(test, 1, 20){
		int n = rng() % 30 + 31, pct = 70 + rng() % 25;
		vector<vector<int>> g(n + 1);
		vector<vector<char>> adj(n + 1, vector<char>(n + 1));
		FOR(u, 1, n) FOR(v, u + 1, n){
			if((int)(rng() % 100) >= pct) continue;
			g[u].push_back(v);
			g[v].push_back(u);
			adj[u][v] = adj[v][u] = 1;
		}
		MaxClique mc(g);
		auto &c = mc.clique;
		assert(!c.empty());
		FOR(i, 0, sz(c) - 1){
			assert(1 <= c[i] && c[i] <= n);
			FOR(j, 0, i - 1){
				assert(c[i] != c[j]);
				assert(adj[c[i]][c[j]]);
			}
		}
	}
}
