#include "../../tex/contents/graph/hld.hpp"
int main(){
	mt19937 rng(1);
	EFOR(int, it, 0, 12000){
		int n = it < 8000 ? rng() % 9 + 1 : rng() % 1000 + 1, shape = rng() % 4, r = rng() % n;
		vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin(), perm.end(), rng);
		HLD h(n); vector<vector<int>> adj(n);
		EFOR(int, i, 1, n){
			int j = shape == 0 ? rng() % i : shape == 1 ? i - 1 : shape == 2 ? (i - 1) / 2 : max(0, i - 1 - (int)(rng() % 3));
			int a = perm[i], b = perm[j];
			if (rng() & 1) h.addEdge(a, b); else h.addEdge(b, a);
			adj[a].push_back(b); adj[b].push_back(a);
		}
		h.build(r);
		// brute: parent/depth from r
		vector<int> par(n, -1), dep(n), ord{r}; vector<bool> vis(n); vis[r] = 1;
		EFOR(int, i, 0, n){ int u = ord[i]; for (int v: adj[u]) if (!vis[v]) vis[v] = 1, par[v] = u, dep[v] = dep[u] + 1, ord.push_back(v); }
		vector<int> bsz(n, 1); EREV(int, i, n - 1, 0) bsz[par[ord[i]]] += bsz[ord[i]];
		auto fail = [&](const char* m){ printf("FAIL %s n=%d r=%d\n", m, n, r); exit(1); };
		// pos permutation
		vector<int> at(n, -1); EFOR(int, v, 0, n){ if (h.pos[v] < 0 || h.pos[v] >= n || at[h.pos[v]] >= 0) fail("pos"); at[h.pos[v]] = v; }
		int lg = 0; while ((1 << (lg + 1)) <= n) lg++;
		vector<ll> val(n); for (auto &x: val) x = rng() % 1000000000;
		EFOR(int, q, 0, n < 10 ? 30 : 20){
			int u = rng() % n, v = rng() % n;
			int a = u, b = v; while (a != b){ if (dep[a] < dep[b]) swap(a, b); a = par[a]; }
			if (h.lca(u, v) != a) fail("lca");
			if (h.dep[u] != dep[u]) fail("dep");
			for (int e = 0; e < 2; e++){
				// brute set of vertices on path (edge mode: skip lca)
				vector<int> cnt(n); ll want = 0;
				for (int x = u; x != a; x = par[x]) cnt[x]++, want += val[x];
				for (int x = v; x != a; x = par[x]) cnt[x]++, want += val[x];
				if (!e) cnt[a]++, want += val[a];
				vector<int> got(n); ll s = 0; int calls = 0;
				h.path(u, v, [&](int l, int rr){ calls++; if (!(0 <= l && l < rr && rr <= n)) fail("range"); EFOR(int, k, l, rr) got[at[k]]++, s += val[at[k]]; }, e);
				if (got != cnt || s != want) fail("path");
				if (calls > 2 * lg + 1) fail("calls");
				// subtree
				auto [l, rr] = h.subtree(u, e);
				// every vertex in [l, rr) has u as a proper/improper ancestor, and the count matches
				if (rr - l != bsz[u] - e) fail("subtree size");
				EFOR(int, k, l, rr){ int y = at[k]; if (e && y == u) fail("subtree edge"); while (y != -1 && y != u) y = par[y]; if (y != u) fail("subtree"); }
			}
		}
	}
	puts("OK");
}
