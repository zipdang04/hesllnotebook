#include "../../tex/contents/graph/hld.hpp"
int main(){
	int n, q; read(n, q);
	HLD h(n);
	EFOR(int, i, 1, n){ int p; read(p); h.addEdge(p, i); }
	h.build();
	string out;
	while (q--){ int u, v; read(u, v); out += to_string(h.lca(u, v)) + '\n'; }
	fputs(out.c_str(), stdout);
}
