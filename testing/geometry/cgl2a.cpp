#include "../../tex/contents/geometry/point.hpp"

int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	int q; cin >> q;
	while (q--) {
		ll v[8]; for (ll &t : v) cin >> t;
		Point<ll> p0(v[0], v[1]), p1(v[2], v[3]), p2(v[4], v[5]), p3(v[6], v[7]);
		cout << (sgn((p1 - p0).cross(p3 - p2)) == 0 ? 2 : sgn((p1 - p0).dot(p3 - p2)) == 0 ? 1 : 0) << '\n';
	}
}
