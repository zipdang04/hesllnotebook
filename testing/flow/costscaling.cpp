#include "../../tex/contents/flow/costscaling.hpp"
// Library Checker min_cost_b_flow: supplies via an extra vertex X with edges fixed to [b, b]
string str(__int128 x) {
	if (x == 0) return "0";
	bool neg = x < 0; if (neg) x = -x;
	string r; while (x) r += char('0' + x % 10), x /= 10;
	if (neg) r += '-';
	return string(r.rbegin(), r.rend());
}
int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<ll> b(n); for (ll &x: b) cin >> x;
	MinCostCirculation mc(n + 1); int X = n;
	vector<int> id(m);
	FOR(int, e, 0, m - 1) { int s, t; ll l, u, c; cin >> s >> t >> l >> u >> c; id[e] = mc.addEdge(s, t, l, u, c); }
	FOR(int, v, 0, n - 1) {
		if (b[v] > 0) mc.addEdge(X, v, b[v], b[v], 0);
		if (b[v] < 0) mc.addEdge(v, X, -b[v], -b[v], 0);
	}
	auto [ok, cost] = mc.solve();
	if (!ok) { cout << "infeasible\n"; return 0; }
	cout << str(cost) << '\n';
	FOR(int, v, 0, n - 1) cout << mc.p[v] << '\n';
	FOR(int, e, 0, m - 1) cout << mc.flowOf(id[e]) << '\n';
}
