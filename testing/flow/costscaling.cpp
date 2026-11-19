#include "../../tex/contents/flow/costscaling.hpp"
#include <cassert>
// Supplies b[v] are fixed edges X -> v with bounds [b, b].
// An extra vertex X balances all supplies and demands.
string str(__int128 x){
	if(x == 0) return "0";
	bool neg = x < 0; string r;
	while(x)
		r += char('0' + abs((int)(x % 10))), x /= 10;
	if(neg) r += '-';
	reverse(all(r)); return r;
}
int main(){
	{
		MinCostCirc mc(1);
		assert(mc.solve() && mc.total == 0);
		MinCostCirc a(2), b(2);
		assert(a.addEdge(0, 1, 1, 2, 5) == 0);
		assert(!a.solve());
		assert(b.addEdge(0, 0, 1, 4, -3) == 0);
		assert(b.addEdge(1, 1, 2, 5, 7) == 2);
		assert(b.addEdge(0, 1, 0, 3, 1) == 4);
		assert(b.addEdge(1, 0, 0, 3, -2) == 6);
		assert(b.adj[0] == vector<int>({4, 7}));
		assert(b.solve());
		assert(b.flowOf(0) == 4 && b.flowOf(2) == 2);
		assert(b.flowOf(4) == 3 && b.flowOf(6) == 3);
		assert(b.total == -1);
		b = MinCostCirc(4);
		assert(sz(b.edges) == 0 && sz(b.pot) == 4);
	}
	{
		// Capacities and costs beyond Library Checker's limits.
		for(auto [cap, cost]: vector<pair<ll, ll>>{
				{4000000000000000000LL, -3},
				{1, -374999999999999999LL}}){
			MinCostCirc mc(2);
			mc.addEdge(0, 1, 0, cap, cost);
			mc.addEdge(1, 0, 0, cap, 0);
			assert(mc.solve());
			assert(mc.flowOf(0) == cap && mc.flowOf(2) == cap);
			assert(mc.total == (__int128)cap * cost);
			FOR(u, 0, 1) for(int id: mc.adj[u])
				if(mc.edges[id].cap > 0)
					assert(mc.reducedCost(u, id) >= 0);
		}
		// Relabels add up along a path: pot < LLONG_MIN / 4.
		ll c = 20454545454545454LL;
		MinCostCirc mc(10);
		FOR(i, 0, 8)
			mc.addEdge(i, i + 1, 0, 10 - i, -c);
		mc.addEdge(9, 0, 0, 1, c);
		assert(mc.solve() && mc.total == (__int128)-8 * c);
		__int128 x = -((__int128)1 << 126);
		assert(str(x + x)
			== "-170141183460469231731687303715884105728");
	}
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<ll> b(n);
	for(ll &x: b)
		cin >> x;
	MinCostCirc mc(n + 1); vector<int> id(m);
	FOR(e, 0, m - 1){
		int s, t; ll l, u, c; cin >> s >> t >> l >> u >> c;
		id[e] = mc.addEdge(s, t, l, u, c);
	}
	FOR(v, 0, n - 1)
		mc.addEdge(n, v, b[v], b[v], 0);
	if(!mc.solve()){
		cout << "infeasible" << '\n'; return 0;
	}
	cout << str(mc.total) << '\n';
	FOR(v, 0, n - 1)
		cout << mc.pot[v] << '\n';
	FOR(e, 0, m - 1)
		cout << mc.flowOf(id[e]) << '\n';
}
