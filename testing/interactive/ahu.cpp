#include "../../tex/contents/interactive/ahu.hpp"
int main(){
	int n; scanf("%d", &n);
	vector<vector<int>> g(n);
	FOR(int, i, 1, n - 1){ int p; scanf("%d", &p); g[p].push_back(i); }
	AHU a; a.rooted(g, 0);
	printf("%d\n", (int)a.mp.size());
	FOR(int, i, 0, n - 1) printf("%d%c", a.id[i], i + 1 < n ? ' ' : '\n');
}
