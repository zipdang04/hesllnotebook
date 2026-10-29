#include "../../tex/contents/graph/hld.hpp"
int main(){
	int n, q; read(n, q);
	vector<ll> a(n), bit(n + 1); for (auto &x: a) read(x);
	auto add = [&](int i, ll x){ for (; i <= n; i += i & -i) bit[i] += x; };
	auto sum = [&](int i){ ll s = 0; for (; i; i -= i & -i) s += bit[i]; return s; };
	HLD h(n);
	FOR(int, i, 1, n - 1){ int p; read(p); h.addEdge(p, i); }
	h.build();
	FOR(int, v, 0, n - 1) add(h.pos[v], a[v]);
	string out;
	while (q--){
		int t, u; read(t, u);
		if (!t){ int x; read(x); add(h.pos[u], x); }
		else { auto [l, r] = h.subtree(u); out += to_string(sum(r) - sum(l - 1)) + '\n'; }
	}
	fputs(out.c_str(), stdout);
}
