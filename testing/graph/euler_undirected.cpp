#include "../../tex/contents/graph/euler.hpp"

int main(){
	int T; scanf("%d", &T);
	Euler E; vector<int> vs, es;
	while (T--){
		int n, m; scanf("%d %d", &n, &m); E.init(n, false);
		FOR(int, i, 0, m - 1){int u, v; scanf("%d %d", &u, &v); E.addEdge(u, v);}
		if (!E.solve(vs, es)){puts("No"); continue;}
		puts("Yes");
		for (int x: vs) printf("%d ", x);
		puts("");
		for (int x: es) printf("%d ", x);
		puts("");
	}
}
