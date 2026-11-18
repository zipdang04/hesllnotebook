#include "../../tex/contents/graph/euler.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	{
		vector<pii> edges = {{1, 2}, {2, 3}, {3, 1}, {1, 1}};
		vector<vector<pii>> g(4);
		FOR(id, 0, 3){
			auto [u, v] = edges[id];
			g[u].push_back({v, id});
			if(u != v)
				g[v].push_back({u, id});
		}
		auto w = eulerWalk(g, 4, 1);
		assert(sz(w) == 5 && w[0] == make_pair(1, -1));
		vector<int> seen(4);
		FOR(i, 1, 4){
			auto [v, id] = w[i]; int u = w[i - 1].fi;
			assert(0 <= id && id < 4 && !seen[id]);
			seen[id] = 1;
			auto [a, b] = edges[id];
			assert((u == a && v == b) || (u == b && v == a));
		}
	}
	int t; cin >> t;
	while(t--){
		int n, m; cin >> n >> m;
		vector<vector<pii>> g(n + 1);
		FOR(id, 0, m - 1){
			int u, v; cin >> u >> v; u++; v++;
			g[u].push_back({v, id});
			g[v].push_back({u, id});
		}
		int s = 1;
		FOR(u, 1, n) if(!g[u].empty())
			s = u;
		FOR(u, 1, n) if(sz(g[u]) % 2)
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
