#include "../../tex/contents/graph/dmst.hpp"
#include <cassert>
int main(){
	DirectedMST dm(vector<vector<pair<int, ll>>>(2), 1);
	assert(dm.cost == 0 && dm.par == vector<int>({0, 0}));
	vector<vector<pair<int, ll>>> g(3);
	g[2].push_back({1, 7}); dm = DirectedMST(g, 1);
	assert(dm.cost == LLONG_MAX);
	g.assign(4, {});
	g[1].push_back({2, 10}); g[2].push_back({3, 1});
	g[3].push_back({2, 1}); dm = DirectedMST(g, 1);
	assert(dm.cost == 11 && dm.par[2] == 1);
	assert(dm.par[3] == 2);
	g[3].push_back({3, -5}); g[1].push_back({2, 4});
	dm = DirectedMST(g, 1);
	assert(dm.cost == 5 && dm.par[2] == 1);
	g.assign(3, {}); g[1].push_back({2, -3});
	DirectedMST other(g, 1);
	assert(other.cost == -3 && other.par[2] == 1);
	g[0].push_back({1, -100}); dm = DirectedMST(g, 1);
	assert(dm.cost == other.cost && dm.par == other.par);
	assert(dm.heap[0].key.w == 0 && dm.heap[0].delta == 0);
	assert(dm.heap[0].l == 0 && dm.heap[0].r == 0);
	for(ll w: {-4000000000000000000LL,
			4000000000000000000LL}){
		g.assign(3, {}); g[1].push_back({2, w});
		dm = DirectedMST(g, 1);
		assert(dm.cost == w);
		assert(dm.par == vector<int>({0, 0, 1}));
	}
	// a legal cost equal to oo is not "unreachable"
	g.assign(3, {}); g[1].push_back({2, oo});
	dm = DirectedMST(g, 1);
	assert(dm.cost == oo && dm.par[2] == 1);
	g[1].push_back({2, 5}); g[2].push_back({2, -7});
	dm = DirectedMST(g, 1);
	assert(dm.cost == 5 && dm.par[2] == 1);
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, m, s; cin >> n >> m >> s; s++;
	g.assign(n + 1, {});
	vector<map<int, ll>> weights(n + 1);
	FOR(i, 1, m){
		int a, b; ll c;
		cin >> a >> b >> c;
		a++; b++;
		g[a].push_back({b, c});
		auto [it, added] = weights[b].emplace(a, c);
		if(!added)
			it->se = min(it->se, c);
	}
	dm = DirectedMST(g, s);
	assert(dm.cost != LLONG_MAX && sz(dm.par) == n + 1);
	assert(dm.par[s] == 0 && dm.par[0] == 0);
	ll sum = 0;
	FOR(v, 1, n) if(v != s){
		assert(weights[v].count(dm.par[v]));
		sum += weights[v].at(dm.par[v]);
	}
	assert(sum == dm.cost);
	vector<int> depth(n + 1, -1); depth[s] = 0;
	FOR(v, 1, n){
		int u = v; vector<int> path;
		while(depth[u] == -1){
			depth[u] = -2; path.push_back(u); u = dm.par[u];
			assert(1 <= u && u <= n);
		}
		assert(depth[u] >= 0);
		int d = depth[u]; reverse(all(path));
		for(int w: path){
			depth[w] = ++d; assert(d < n);
		}
	}
	cout << dm.cost << '\n';
	FOR(v, 1, n)
		cout << (v == s ? s - 1 : dm.par[v] - 1) << ' ';
	cout << '\n';
}
