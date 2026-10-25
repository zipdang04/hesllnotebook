#include "../../tex/contents/graph/maxclique.hpp"

int main(){
	int n, m; scanf("%d %d", &n, &m);
	vector<vector<bool>> a(n, vector<bool>(n));
	EFOR(int, i, 0, m){ int u, v; scanf("%d %d", &u, &v); a[u][v] = a[v][u] = 1; }
	MaxClique<64> mc(n);
	EFOR(int, i, 0, n) EFOR(int, j, i + 1, n) if (!a[i][j]) mc.addEdge(i, j);
	auto r = mc.solve();
	assert(mc.solve() == r);
	printf("%d\n", (int)r.size());
	EFOR(int, i, 0, r.size()) printf("%d%c", r[i], i + 1 < (int)r.size() ? ' ' : '\n');
}
