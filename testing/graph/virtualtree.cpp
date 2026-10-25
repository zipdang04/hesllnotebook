#include "../../tex/contents/graph/virtualtree.hpp"
int main(){
	int n; scanf("%d", &n);
	HLD h(n); vector<vector<pair<int, int>>> g(n);
	EFOR(int, i, 1, n){ int u, v, w; scanf("%d %d %d", &u, &v, &w); h.addEdge(u, v); g[u].push_back({v, w}); g[v].push_back({u, w}); }
	h.build(0);
	vector<ll> wd(n);
	vector<int> ord{0};
	EFOR(int, i, 0, n){ int u = ord[i]; for (auto [v, w]: g[u]) if (v != h.par[u]) wd[v] = wd[u] + w, ord.push_back(v); }
	VirtualTree vt(h);
	int q; scanf("%d", &q);
	while (q--){
		int k; scanf("%d", &k); vector<int> S(k); for (int &x: S) scanf("%d", &x);
		vt.build(S); ll s = 0;
		EFOR(int, i, 1, (int)vt.vs.size()) s += wd[vt.vs[i]] - wd[vt.vs[vt.pa[i]]];
		printf("%lld\n", s);
	}
}
