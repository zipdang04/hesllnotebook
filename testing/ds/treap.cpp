#include "../../tex/contents/ds/treap.hpp"
#include "../../tex/contents/math/modint.hpp"

struct Affine{ // S = (sum, len), F = x -> b x + c
	using S = pair<modint, int>; using F = pair<modint, modint>;
	static S op(S x, S y){ return {x.fi + y.fi, x.se + y.se}; }
	static S e(){ return {0, 0}; }
	static S mp(F f, S x){ return {f.fi * x.fi + f.se * x.se, x.se}; }
	static F cp(F f, F g){ return {f.fi * g.fi, f.fi * g.se + f.se}; }
	static F id(){ return {1, 0}; }
};
int main(){
	int n, q; read(n, q);
	vector<Affine::S> a(n);
	for (auto& x: a){ int v; read(v); x = {v, 1}; }
	Treap<Affine> tr(a);
	string out;
	while (q--){
		int ty, l, r, b, c; read(ty);
		if (ty == 0){ read(l, b); tr.insert(l + 1, {b, 1}); }
		else if (ty == 1){ read(l); tr.erase(l + 1); }
		else if (ty == 2){ read(l, r); tr.reverse(l + 1, r); }
		else if (ty == 3){ read(l, r, b, c); tr.apply(l + 1, r, {b, c}); }
		else { read(l, r); out += to_string(tr.query(l + 1, r).fi.v); out += '\n'; }
	}
	fputs(out.c_str(), stdout);
}
