#include "../../tex/contents/graph/scc.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	// n = 0, and reassignment compiles
	SCC empty(vector<vector<int>>(1));
	assert(empty.cnt == 0);
	empty = SCC(vector<vector<int>>(2));
	assert(empty.cnt == 1);
	int n, m; cin >> n >> m;
	vector<vector<int>> g(n + 1);
	FOR(i, 1, m){
		int u, v; cin >> u >> v;
		g[u + 1].push_back(v + 1);
	}
	SCC scc(g);
	assert(scc.comp[0] == 0);
	vector<vector<int>> group(scc.cnt + 1);
	FOR(u, 1, n){
		assert(1 <= scc.comp[u] && scc.comp[u] <= scc.cnt);
		group[scc.comp[u]].push_back(u);
	}
	cout << scc.cnt << '\n';
	REV(c, scc.cnt, 1){
		cout << group[c].size();
		for(int u: group[c]) cout << ' ' << u - 1;
		cout << '\n';
	}
}
