#include "../../tex/contents/ds/lct.hpp"
int main(){
	mt19937 rng(1);
	auto rnd = [&](int a, int b){ return (int)(rng() % (b - a + 1)) + a; };
	EFOR(int, seed, 0, 3000){
		int n = rnd(1, 12); LCT lct(n);
		vector<ll> a(n, 0); vector<set<int>> g(n); vector<pair<int,int>> edges;
		auto bfs = [&](int r, vector<int>& par, vector<int>& dep){ // par = -2 if unreachable
			par.assign(n, -2); dep.assign(n, 0); par[r] = -1; queue<int> qu; qu.push(r);
			while (qu.size()){ int u = qu.front(); qu.pop(); for (int v: g[u]) if (par[v] == -2) par[v] = u, dep[v] = dep[u] + 1, qu.push(v); }
		};
		auto fail = [&](const char* what){ printf("seed %d: %s\n", seed, what); exit(1); };
		EFOR(int, it, 0, 300){
			int ty = rnd(0, 5), u = rnd(0, n - 1), v = rnd(0, n - 1), r = rnd(0, n - 1);
			vector<int> par, dep; bfs(u, par, dep);
			if (ty == 0){ // link if possible
				if (par[v] == -2){ lct.link(u, v); g[u].insert(v); g[v].insert(u); edges.push_back({u, v}); }
			} else if (ty == 1){ // cut a random edge (given in random orientation)
				if (edges.size()){ int k = rnd(0, edges.size() - 1); auto [x, y] = edges[k]; if (rng() & 1) swap(x, y);
					lct.cut(x, y); g[x].erase(y); g[y].erase(x); edges.erase(edges.begin() + k); }
			} else if (ty == 2){ // set, values near 1e18 / n to test ll range
				ll x = rng() & 1 ? (ll)(rng() % 1000) - 500 : (ll)8e16 - rnd(0, 1000); lct.set(u, x); a[u] = x;
			} else if (ty == 3){
				if (lct.connected(u, v) != (par[v] != -2)) fail("connected");
			} else if (ty == 4){
				if (par[v] == -2) continue;
				ll s = 0; for (int x = v; x != -1; x = par[x]) s += a[x];
				if (lct.query(u, v) != s) fail("query");
			} else {
				bfs(r, par, dep); if (par[u] == -2 || par[v] == -2) continue;
				int x = u, y = v; while (x != y){ if (dep[x] < dep[y]) swap(x, y); x = par[x]; }
				if (lct.lca(r, u, v) != x) fail("lca");
				// findRoot after makeRoot(r) via lca
				if (lct.findRoot(u) != r) fail("findRoot");
			}
		}
	}
	puts("OK");
}
