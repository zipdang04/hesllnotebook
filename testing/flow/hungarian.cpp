#include "../../tex/contents/flow/hungarian.hpp"

int main(){
	int n; scanf("%d", &n);
	Hungarian h(n, n);
	EFOR(int, i, 0, n) EFOR(int, j, 0, n) scanf("%lld", &h.c[i][j]);
	auto [x, p] = h.run();
	printf("%lld\n", x);
	EFOR(int, i, 0, n) printf("%d%c", p[i], " \n"[i == n - 1]);
}
