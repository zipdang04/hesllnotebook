#include "../../tex/contents/graph/scc.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m; SCC s(n);
	FOR(int, i, 0, m - 1){ int a, b; cin >> a >> b; s.addEdge(a, b); }
	int k = s.run(); vector<vector<int>> grp(k);
	FOR(int, v, 0, n - 1) grp[s.comp[v]].push_back(v);
	cout << k << '\n';
	for (auto &c: grp){ cout << c.size(); for (int v: c) cout << ' ' << v; cout << '\n'; }
}
