#include "../../tex/contents/graph/spfa.hpp"
#include <cassert>
int main(){
	vector<vector<pair<int, ll>>> g(3);
	SPFA a(g, 1);
	assert(a.dist[1] == 0 && a.dist[2] == oo);
	assert(a.dist[0] == oo && a.par[0] == 0);
	assert(a.par[1] == 0 && a.par[2] == 0);
	assert(!a.negCycle);
	g.resize(2); g[1].push_back({1, -1});
	SPFA b(g, 1);
	assert(b.dist[1] == -oo && b.negCycle);
	g = {{}, {{2, 5}}, {}, {{4, -1}}, {{3, -1}}};
	SPFA c(g, 1);
	assert(!c.negCycle && c.dist[2] == 5);
	assert(c.dist[3] == oo && c.dist[4] == oo);
	assert(c.par[2] == 1 && c.par[3] == 0);
	g = {{}, {{2, 1}, {5, 7}}, {{3, -2}},
		{{2, 1}, {4, 0}}, {}, {}};
	SPFA d(g, 1);
	assert(d.negCycle && d.dist[2] == -oo);
	assert(d.dist[3] == -oo && d.dist[4] == -oo);
	assert(d.dist[5] == 7 && d.par[5] == 1);
	vector<vector<pair<int, ll>>> g2(3);
	g2[0].push_back({1, -100});
	g2[1] = {{2, -3}, {2, 4}};
	a = SPFA(g2, 1);
	assert(!a.negCycle && a.dist[0] == oo);
	assert(a.dist[1] == 0 && a.par[1] == 0);
	assert(a.dist[2] == -3 && a.par[2] == 1);
	assert(b.dist[1] == -oo && b.negCycle);
	g = {{}, {{2, -499999999999999999LL}}, {}};
	SPFA e(g, 1);
	assert(e.dist[2] == -499999999999999999LL);
	assert(!e.negCycle && e.par[2] == 1);
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, m, s, t; cin >> n >> m >> s >> t;
	s++; t++;
	g.assign(n + 1, {});
	map<pii, ll> weight;
	FOR(i, 1, m){
		int u, v; ll w; cin >> u >> v >> w;
		u++; v++;
		g[u].push_back({v, w});
		auto it = weight.find({u, v});
		if(it == weight.end())
			weight[{u, v}] = w;
		else
			it->se = min(it->se, w);
	}
	mt19937 rng(712367);
	FOR(u, 1, n)
		shuffle(all(g[u]), rng);
	SPFA sp(g, s);
	assert(!sp.negCycle && sp.par[s] == 0);
	if(sp.dist[t] == oo){
		cout << -1 << '\n'; return 0;
	}
	vector<pii> path; ll sum = 0;
	for(int v = t; v != s; v = sp.par[v]){
		assert(sz(path) < n - 1);
		int u = sp.par[v];
		auto it = weight.find({u, v});
		assert(it != weight.end());
		sum += it->se; path.push_back({u, v});
	}
	assert(sum == sp.dist[t]);
	reverse(all(path));
	cout << sp.dist[t] << ' ' << sz(path) << '\n';
	for(auto [u, v]: path)
		cout << u - 1 << ' ' << v - 1 << '\n';
}
