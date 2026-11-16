#include "../../tex/contents/ds/lichao.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q;
	cin >> n >> q;
	vector<array<ll, 3>> ops;
	vector<ll> pts = {0};
	FOR(i, 1, n){
		ll a, b;
		cin >> a >> b;
		ops.push_back({0, a, b});
	}
	FOR(i, 1, q){
		int t;
		cin >> t;
		if(t == 0){
			ll a, b;
			cin >> a >> b;
			ops.push_back({0, a, b});
		}else{
			ll p;
			cin >> p;
			ops.push_back({1, p, 0});
			pts.push_back(p);
		}
	}
	LiChao lc(pts);
	for(auto [t, x, y]: ops){
		if(t == 0) lc.addLine(x, y);
		else cout << lc.get(x) << "\n";
	}
}
