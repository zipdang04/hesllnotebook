#include "../../tex/contents/ds/lct.hpp"
int main(){
	int n, q; read(n, q);
	LCT lct(n);
	FOR(int, i, 0, n - 1){ ll a; read(a); lct.set(i, a); }
	FOR(int, i, 0, n - 2){ int u, v; read(u, v); lct.link(u, v); }
	string out;
	while (q--){
		int ty; read(ty);
		if (ty == 0){ int u, v, w, x; read(u, v, w, x); lct.cut(u, v); lct.link(w, x); }
		else if (ty == 1){ int p; ll x; read(p, x); lct.set(p, lct.t[p].v + x); }
		else { int u, v; read(u, v); out += to_string(lct.query(u, v)) + '\n'; }
	}
	fputs(out.c_str(), stdout);
}
