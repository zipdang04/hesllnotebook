#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
bool lt(P a, P b){ return abs(a.x - b.x) > 1e-9 ? a.x < b.x : a.y < b.y; }

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld v[6]; for (ld &t: v) cin >> t;
	P a(v[0], v[1]), b(v[2], v[3]), c(v[4], v[5]), o = ccCenter(a, b, c);
	cout << o.x << ' ' << o.y << ' ' << (a - o).dist() << '\n';
}
