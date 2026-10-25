#include "../../tex/contents/geometry/polygon.hpp"

int main(){
	int n, q; cin >> n;
	vector<Point<ld>> v(n);
	for (auto& p: v) cin >> p.x >> p.y;
	cin >> q;
	cout << fixed << setprecision(10);
	while (q--){
		Point<ld> s, e; cin >> s.x >> s.y >> e.x >> e.y;
		auto r = polygonCut(v, s, e);
		cout << (r.size() < 3 ? 0 : polyArea2(r) / 2) << '\n';
	}
}
