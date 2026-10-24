#include "../../tex/contents/flow/mcmf.hpp"

// AOJ GRL_6_B: min cost to send exactly F units from 0 to V-1, or -1
int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; ll f; cin >> n >> m >> f;
	MCMF<> g; g.init(n, 0, n - 1);
	FOR(int, _, 1, m) {
		int u, v; ll c, d; cin >> u >> v >> c >> d;
		g.addEdge(u, v, c, d);
	}
	auto [flow, cost] = g.calculate(f);
	cout << (flow < f ? -1 : cost) << '\n';
}
