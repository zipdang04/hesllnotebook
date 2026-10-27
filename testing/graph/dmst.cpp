#include "../../tex/contents/graph/dmst.hpp"
int main(){
	int n, m, s; scanf("%d%d%d", &n, &m, &s); DMST d(n);
	while (m--){ int a, b; ll c; scanf("%d%d%lld", &a, &b, &c); d.addEdge(a, b, c); }
	auto [x, p] = d.solve(s); p[s] = s;
	printf("%lld\n", x);
	for (int v: p) printf("%d ", v);
	puts("");
}
