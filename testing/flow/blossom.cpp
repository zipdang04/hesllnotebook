#include "../../tex/contents/flow/blossom.hpp"
int main(){ int n, m; read(n, m); Blossom b(n); FOR(int, i, 0, m - 1){int u,v; read(u,v); b.addEdge(u,v);}
	printf("%d\n", b.run()); FOR(int, i, 0, n - 1) if (b.mate[i] < i) printf("%d %d\n", b.mate[i], i); }
