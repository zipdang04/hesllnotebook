#include "../../tex/contents/ds/lichao.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q;
	cin >> n >> q;
	vector<array<ll, 5>> ops;
	vector<ll> pts = {0};
	FOR(i, 1, n){
		ll l, r, a, b;
		cin >> l >> r >> a >> b;
		ops.push_back({0, l, r, a, b});
	}
	FOR(i, 1, q){
		int t;
		cin >> t;
		if(t == 0){
			ll l, r, a, b;
			cin >> l >> r >> a >> b;
			ops.push_back({0, l, r, a, b});
		}else{
			ll p;
			cin >> p;
			ops.push_back({1, p, 0, 0, 0});
			pts.push_back(p);
		}
	}
	LiChao lc(pts);
	lc.addSegment(1, 1, 1, -1); // l > r: must not cover x = 0
	assert(lc.get(0) == LiChao::oo);
	for(auto [t, l, r, a, b]: ops){
		if(t == 0) lc.addSegment(a, b, l, r - 1);
		else{
			ll v = lc.get(l);
			if(v == LiChao::oo) cout << "INFINITY\n";
			else cout << v << "\n";
		}
	}
}
