#include "../../tex/contents/graph/virtualtree.hpp"
#include <cassert>

int main(){
	{
		vector<vector<int>> g(6);
		vector<pii> edges = {{1, 2}, {1, 3}, {3, 4}, {3, 5}};
		for(auto [u, v]: edges){
			g[u].push_back(v); g[v].push_back(u);
		}
		HLD hld(g);
		assert(virtualTree(hld, {}).empty());
		vector<pii> one = {{3, 0}};
		assert(virtualTree(hld, {3, 3}) == one);
		vector<pii> want = {
			{1, 0}, {3, 1}, {4, 3}, {5, 3}, {2, 1}
		};
		auto t = virtualTree(hld, {2, 4, 5});
		assert(t == want);
		int sum = 0;
		for(auto [v, p]: t) if(p)
			sum += hld.depth[v] - hld.depth[p];
		assert(sum == 4);
		t = virtualTree(hld, {5, 4, 3, 2, 1});
		assert(sz(t) == 5);
		for(auto [v, p]: t)
			assert(p == hld.par[v]);
		vector<vector<int>> g2(4);
		FOR(v, 2, 3){
			g2[v-1].push_back(v); g2[v].push_back(v - 1);
		}
		HLD hld2(g2, 3);
		vector<pii> want2 = {{3, 0}, {1, 3}};
		assert(virtualTree(hld2, {1, 3, 1}) == want2);
		assert(virtualTree(hld, {2, 4, 5}) == want);
		assert(virtualTree(hld2, {1, 3}) == want2);
	}
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n; cin >> n;
	vector<vector<int>> g(n + 1);
	vector<vector<pair<int, ll>>> wg(n + 1);
	FOR(i, 1, n - 1){
		int u, v; ll w; cin >> u >> v >> w; u++; v++;
		g[u].push_back(v); g[v].push_back(u);
		wg[u].push_back({v, w}); wg[v].push_back({u, w});
	}
	vector<ll> dist(n + 1);
	vector<int> par(n + 1);
	queue<int> q; q.push(1);
	while(!q.empty()){
		int u = q.front(); q.pop();
		for(auto [v, w]: wg[u]) if(v != par[u]){
			par[v] = u; dist[v] = dist[u] + w; q.push(v);
		}
	}
	HLD hld(g);
	int queries; cin >> queries;
	while(queries--){
		int k; cin >> k;
		vector<int> xs(k);
		for(int &v: xs){
			cin >> v; v++;
		}
		auto t = virtualTree(hld, xs);
		assert(!t.empty() && t[0].se == 0);
		ll ans = 0;
		FOR(i, 0, sz(t) - 1){
			auto [v, p] = t[i];
			if(i){
				assert(p != 0);
				assert(hld.pos[t[i-1].fi] < hld.pos[v]);
			}
			if(p)
				ans += dist[v] - dist[p];
		}
		cout << ans << '\n';
	}
}
