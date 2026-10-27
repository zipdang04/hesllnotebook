#include "../../tex/contents/geometry/hull.hpp"

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int T; cin >> T;
	while (T--){
		int n; cin >> n;
		vector<Point<ll>> v(n);
		for (auto& p: v) cin >> p.x >> p.y;
		if (n == 0){ cout << "0\n"; continue; }
		auto h = convexHull(v);
		cout << h.size() << '\n';
		for (auto& p: h) cout << p.x << ' ' << p.y << '\n';
	}
}
