#include "../../tex/contents/graph/hld.hpp"
int main(){
	int n, q; read(n, q);
	vector<ll> a(n), bit(n + 1); for (auto &x: a) read(x);
	auto add = [&](int i, ll x){ for (; i <= n; i += i & -i) bit[i] += x; };
	auto sum = [&](int i){ ll s = 0; for (; i; i -= i & -i) s += bit[i]; return s; };
	HLD h(n);
	FOR(int, i, 1, n - 1){ int u, v; read(u, v); h.addEdge(u, v); }
	h.build();
	FOR(int, v, 0, n - 1) add(h.pos[v], a[v]);
	string out;
	while (q--){
		int t, u, v; read(t, u, v);
		if (!t) add(h.pos[u], v);
		else { ll s = 0; h.path(u, v, [&](int l, int r){ s += sum(r) - sum(l - 1); }); out += to_string(s) + '\n'; }
	}
	fputs(out.c_str(), stdout);
}
