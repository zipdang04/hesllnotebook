#include "../../tex/contents/geometry/closestpair.hpp"

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int T; cin >> T;
	while (T--){
		int n; cin >> n;
		vector<Point<ll>> v(n);
		for (auto& p: v) cin >> p.x >> p.y;
		auto [a, b] = closestPair(v);
		int i = 0, j = 0;
		while (!(v[i] == a)) i++;
		while (j == i || !(v[j] == b)) j++;
		cout << i << ' ' << j << '\n';
	}
}
