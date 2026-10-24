#include "../../tex/contents/flow/costscaling.hpp"
// AOJ GRL_6_B via circulation: edge t -> s with bounds [F, F]
int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; ll f; cin >> n >> m >> f;
	MinCostCirculation mc(n);
	FOR(int, _, 1, m) { int u, v; ll c, d; cin >> u >> v >> c >> d; mc.addEdge(u, v, 0, c, d); }
	mc.addEdge(n - 1, 0, f, f, 0);
	auto [ok, cost] = mc.solve();
	cout << (ok ? (ll)cost : -1) << '\n';
}
