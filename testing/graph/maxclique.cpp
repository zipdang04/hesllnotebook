#include "../../tex/contents/graph/maxclique.hpp"

int main(){
	int n, m; scanf("%d %d", &n, &m);
	vector<vector<bool>> a(n, vector<bool>(n));
	FOR(int, i, 0, m - 1){ int u, v; scanf("%d %d", &u, &v); a[u][v] = a[v][u] = 1; }
	MaxClique<64> mc(n);
	FOR(int, i, 0, n - 1) FOR(int, j, i + 1, n - 1) if (!a[i][j]) mc.addEdge(i, j);
	auto r = mc.solve();
	assert(mc.solve() == r);
	printf("%d\n", (int)r.size());
	FOR(int, i, 0, (int)r.size() - 1) printf("%d%c", r[i], i + 1 < (int)r.size() ? ' ' : '\n');
}
