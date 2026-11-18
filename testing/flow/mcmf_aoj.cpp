#include "../../tex/contents/flow/mcmf.hpp"
#include <cassert>
int main(){
	MCMF a(3);
	assert((a.maxFlow(1, 2) == pll(0, 0)));
	assert((a.maxFlow(1, 1) == pll(0, 0)));
	assert(a.addEdge(1, 1, 5, 3) == 0);
	assert((a.maxFlow(1, 2) == pll(0, 0)));
	MCMF b(3);
	assert(b.addEdge(0, 1, 1, -5) == 0);
	assert(b.addEdge(1, 2, 1, 2) == 2);
	assert(b.addEdge(0, 2, 1, 0) == 4);
	MCMF c = b;
	assert((b.maxFlow(0, 2) == pll(2, -3)));
	FOR(id, 0, 2)
		assert(b.flowOf(2 * id) == 1);
	assert((b.maxFlow(0, 2) == pll(0, 0)));
	assert((c.maxFlow(0, 2, 1) == pll(1, -3)));
	assert((c.maxFlow(0, 2) == pll(1, 0)));
	b = MCMF(5); assert(sz(b.edges) == 0);
	MCMF d(2);
	d.addEdge(0, 1, 1, 5);
	assert((d.maxFlow(0, 1) == pll(1, 5)));
	assert((d.maxFlow(1, 0) == pll(1, -5)));
	MCMF big(2);
	big.addEdge(0, 1, 1, oo);
	assert((big.maxFlow(0, 1, 1) == pll(1, oo)));
	big = MCMF(2);
	ll large = -5000000000000000000LL;
	big.addEdge(0, 1, 1, large);
	big.addEdge(1, 1, 0, large);
	assert((big.maxFlow(0, 1, 1) == pll(1, large)));
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; ll f; cin >> n >> m >> f;
	MCMF mf(n);
	FOR(i, 1, m){
		int u, v; ll cap, cost;
		cin >> u >> v >> cap >> cost;
		mf.addEdge(u, v, cap, cost);
	}
	auto [flow, cost] = mf.maxFlow(0, n - 1, f);
	cout << (flow < f ? -1 : cost) << '\n';
}
