#include "../../tex/contents/graph/dominator.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m, s; cin >> n >> m >> s; Dominator d(n);
	FOR(int, i, 0, m - 1){ int a, b; cin >> a >> b; d.addEdge(a, b); }
	auto p = d.run(s);
	FOR(int, i, 0, n - 1) cout << p[i] << " \n"[i == n - 1];
}
