#include "../../tex/contents/geometry/point.hpp"

int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld x1, y1, x2, y2; int q; cin >> x1 >> y1 >> x2 >> y2 >> q;
	Point<ld> a(x1, y1), b(x2, y2);
	while (q--) {
		ld x, y; cin >> x >> y;
		Point<ld> r = lineProj(a, b, Point<ld>(x, y));
		cout << r.x << ' ' << r.y << '\n';
	}
}
