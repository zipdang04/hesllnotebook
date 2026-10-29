#include "../../tex/contents/graph/virtualtree.hpp"
int main(){
	mt19937 rng(7);
	FOR(int, it, 0, 11999){
		int n = it % 2 ? rng() % 12 + 1 : rng() % 100 + 1, shape = rng() % 3, r = rng() % n;
		vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin(), perm.end(), rng);
		HLD h(n); vector<vector<int>> adj(n);
		FOR(int, i, 1, n - 1){
			int j = shape == 0 ? rng() % i : shape == 1 ? i - 1 : max(0, i - 1 - (int)(rng() % 3));
			int a = perm[i], b = perm[j];
			if (rng() & 1) h.addEdge(a, b); else h.addEdge(b, a);
			adj[a].push_back(b); adj[b].push_back(a);
		}
		h.build(r);
		vector<int> par(n, -1), dep(n), ord{r};
		FOR(int, i, 0, n - 1){ int u = ord[i]; for (int v: adj[u]) if (v != par[u]) par[v] = u, dep[v] = dep[u] + 1, ord.push_back(v); }
		auto lca = [&](int a, int b){ while (a != b){ if (dep[a] < dep[b]) swap(a, b); a = par[a]; } return a; };
		VirtualTree vt(h);
		FOR(int, rep, 0, 4){
			int k = rng() % 4 == 0 ? 1 : rng() % (rng() & 1 ? n : min(n, 6)) + 1;
			vector<int> S(k); for (int &x: S) x = rng() % n;
			vt.build(S);
			auto &vs = vt.vs; int m = vs.size(); bool ok = m >= 1 && vt.pa[0] == -1 && 1;
			set<int> cl; int top = S[0]; for (int a: S) for (int b: S) cl.insert(lca(a, b)), top = lca(top, a);
			ok = ok && vs[0] == top;
			ok = ok && (int)cl.size() == m && m <= 2 * k - 1;
			FOR(int, i, 0, m - 1) if (ok){
				ok = cl.count(vs[i]) && vt.id[vs[i]] == i && (!i || (h.pos[vs[i - 1]] < h.pos[vs[i]] && vt.pa[i] >= 0 && vt.pa[i] < i));
				if (!ok || !i) continue;
				int x = par[vs[i]]; while (x >= 0 && !cl.count(x)) x = par[x];
				ok = x == vs[vt.pa[i]] && dep[vs[i]] > dep[x];
			}
			// pairwise distances through the compressed tree
			if (ok) for (int a: S){
				int ia = vt.id[a];
				for (int b: S){
					int ib = vt.id[b], x = ia, y = ib; ll d = 0;
					while (x != y){ if (x < y) swap(x, y); d += dep[vs[x]] - dep[vs[vt.pa[x]]]; x = vt.pa[x]; }
					if (d != dep[a] + dep[b] - 2 * dep[lca(a, b)]) ok = false;
				}
			}
			if (!ok){
				printf("FAIL n=%d r=%d S:", n, r); for (int x: S) printf(" %d", x); puts("");
				return 1;
			}
		}
	}
	puts("OK");
}
