#include "../../tex/contents/flow/hungarian.hpp"

int main(){
	int n; scanf("%d", &n);
	Hungarian h(n, n);
	FOR(int, i, 0, n - 1) FOR(int, j, 0, n - 1) scanf("%lld", &h.c[i][j]);
	auto [x, p] = h.run();
	printf("%lld\n", x);
	FOR(int, i, 0, n - 1) printf("%d%c", p[i], " \n"[i == n - 1]);
}
