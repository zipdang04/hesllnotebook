#include "../../tex/contents/ds/lazyseg.hpp"
#include "../../tex/contents/math/modint.hpp"
struct Affine{ // S = (sum, len), F = x -> f.fi * x + f.se
	using S = pair<modint, int>; using F = pair<modint, modint>;
	static S op(S x, S y){ return {x.fi + y.fi, x.se + y.se}; }
	static S e(){ return {0, 0}; }
	static S mp(F f, S x){ return {f.fi * x.fi + f.se * x.se, x.se}; }
	static F cp(F f, F g){ return {f.fi * g.fi, f.fi * g.se + f.se}; }
	static F id(){ return {1, 0}; }
};
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	vector<Affine::S> a(n);
	for (auto& x: a){ ll v; cin >> v; x = {v, 1}; }
	LazySeg<Affine> seg(a);
	string out;
	while (q--){
		int t, l, r; cin >> t >> l >> r;
		if (t == 0){ ll b, c; cin >> b >> c; seg.apply(l + 1, r, {b, c}); } // half-open 0-idx [l, r) = 1-idx [l+1, r]
		else out += to_string(seg.query(l + 1, r).fi.v) + '\n';
	}
	cout << out;
}
