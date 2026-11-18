#include "../../tex/contents/graph/spfa.hpp"
// AOJ GRL_1_B: negative edges and negative cycles.
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, m, s; cin >> n >> m >> s;
	s++;
	vector<vector<pair<int, ll>>> g(n + 1);
	FOR(i, 1, m){
		int u, v; ll w; cin >> u >> v >> w;
		g[u+1].push_back({v + 1, w});
	}
	SPFA sp(g, s);
	if(sp.negCycle){
		cout << "NEGATIVE CYCLE\n"; return 0;
	}
	FOR(v, 1, n){
		if(sp.dist[v] == oo)
			cout << "INF\n";
		else
			cout << sp.dist[v] << '\n';
	}
}
