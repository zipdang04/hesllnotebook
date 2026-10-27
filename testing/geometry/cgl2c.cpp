#include "../../tex/contents/geometry/point.hpp"

int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	int q; cin >> q;
	while (q--) {
		ll v[8]; for (ll &t : v) cin >> t;
		Point<ld> p0(v[0], v[1]), p1(v[2], v[3]), p2(v[4], v[5]), p3(v[6], v[7]);
		auto r = lineInter(p0, p1, p2, p3); assert(r.first == 1);
		auto w = segInter(p0, p1, p2, p3); assert(w.size() == 1 && (w[0] - r.second).dist() < 1e-6);
		cout << r.second.x << ' ' << r.second.y << '\n';
	}
}
