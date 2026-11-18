#include "../../tex/contents/graph/euler.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	{
		vector<vector<pii>> g(2);
		assert((eulerWalk(g, 0, 1) == vector<pii>{{1, -1}}));
		g.resize(4);
		g[1] = {{2, 0}}; g[2] = {{1, 1}};
		vector<pii> want = {{1, -1}, {2, 0}, {1, 1}};
		assert(eulerWalk(g, 2, 1) == want);
		g[0] = {{1, 0}};
		assert(eulerWalk(g, 2, 1) == want);
		g[1] = {{2, 1}}; g[2] = {{1, 2}};
		want = {{1, -1}, {2, 1}, {1, 2}};
		assert(eulerWalk(g, 2, 1) == want);
		g[1] = {{2, 0}}; g[2] = {{1, 2}};
		want = {{1, -1}, {2, 0}, {1, 2}};
		assert(eulerWalk(g, 2, 1) == want);
		g[1] = {{2, 0}, {3, 1}}; g[2].clear();
		assert(eulerWalk(g, 2, 1).empty());
		g[1] = {{1, 0}}; g[2] = {{2, 1}};
		assert(eulerWalk(g, 2, 1).empty());
	}
	int t; cin >> t;
	while(t--){
		int n, m; cin >> n >> m;
		vector<vector<pii>> g(n + 1);
		vector<int> D(n + 1);
		FOR(id, 0, m - 1){
			int u, v; cin >> u >> v; u++; v++;
			g[u].push_back({v, id}); D[u]++; D[v]--;
		}
		int s = 1;
		FOR(u, 1, n) if(!g[u].empty())
			s = u;
		FOR(u, 1, n) if(D[u] == 1)
			s = u;
		auto w = eulerWalk(g, m, s);
		if(w.empty()){
			cout << "No" << '\n'; continue;
		}
		assert(sz(w) == m + 1 && w[0] == make_pair(s, -1));
		cout << "Yes" << '\n';
		for(auto [v, id]: w)
			cout << v - 1 << ' ';
		cout << '\n';
		FOR(i, 1, m)
			cout << w[i].se << ' ';
		cout << '\n';
	}
}
