#include "../../tex/contents/geometry/point.hpp"

int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	ll x0, y0, x1, y1; int q; cin >> x0 >> y0 >> x1 >> y1 >> q;
	Point<ll> a(x0, y0), b(x1, y1);
	while (q--) {
		ll x, y; cin >> x >> y;
		Point<ll> c(x, y);
		int o = orient(a, b, c);
		if (o > 0) cout << "COUNTER_CLOCKWISE\n";
		else if (o < 0) cout << "CLOCKWISE\n";
		else if (onSegment(a, b, c)) cout << "ON_SEGMENT\n";
		else if ((c - a).dot(b - a) < 0) cout << "ONLINE_BACK\n";
		else cout << "ONLINE_FRONT\n";
	}
}
