#include "../../tex/contents/flow/blossom.hpp"
int main(){ int n, m; read(n, m); Blossom b(n); EFOR(int,i,0,m){int u,v; read(u,v); b.addEdge(u,v);}
	printf("%d\n", b.run()); EFOR(int,i,0,n) if (b.mate[i] < i) printf("%d %d\n", b.mate[i], i); }
