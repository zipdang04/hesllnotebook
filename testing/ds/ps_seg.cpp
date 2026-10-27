#include "../../tex/contents/ds/ps_seg.hpp"
int main(){
	int n, q; read(n, q);
	vector<array<int, 3>> p(n); // {x, y, w}
	vector<int> ys;
	for (auto &a: p) read(a[0], a[1], a[2]), ys.push_back(a[1]);
	sort(p.begin(), p.end());
	sort(ys.begin(), ys.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());
	auto rk = [&](int y){ return int(lower_bound(ys.begin(), ys.end(), y) - ys.begin()); };
	PSeg<ll> t(ys.size()); t.L.reserve(n * 20 + 9); t.R.reserve(n * 20 + 9); t.v.reserve(n * 20 + 9);
	vector<int> root{0};
	for (auto [x, y, w]: p){
		int k = rk(y);
		root.push_back(t.upd(root.back(), k, t.qry(root.back(), k, k + 1) + w));
	}
	auto cnt = [&](int x, int d, int u){ // sum over points with x' < x, y in [d, u)
		int k = lower_bound(p.begin(), p.end(), array<int, 3>{x, -1, -1}) - p.begin();
		return t.qry(root[k], rk(d), rk(u));
	};
	while (q--){
		int l, d, r, u; read(l, d, r, u);
		printf("%lld\n", cnt(r, d, u) - cnt(l, d, u));
	}
}
