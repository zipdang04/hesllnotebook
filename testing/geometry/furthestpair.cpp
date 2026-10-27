#include "../../tex/contents/geometry/hull.hpp"

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int T; cin >> T;
	while (T--){
		int n; cin >> n;
		vector<Point<ll>> v(n);
		for (auto& p: v) cin >> p.x >> p.y;
		auto d = hullDiameter(convexHull(v));
		int i = 0, j = 0;
		while (!(v[i] == d[0])) i++;
		while (j == i || !(v[j] == d[1])) j++;
		cout << i << ' ' << j << '\n';
	}
}
