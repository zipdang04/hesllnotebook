#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
bool lt(P a, P b){ return abs(a.x - b.x) > 1e-9 ? a.x < b.x : a.y < b.y; }

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld x, y, r; cin >> x >> y >> r;
	int q; cin >> q;
	while (q--){
		ld v[4]; for (ld &t: v) cin >> t;
		auto s = circleLine(P(x, y), r, P(v[0], v[1]), P(v[2], v[3]));
		if (s.size() == 1) s.push_back(s[0]);
		sort(s.begin(), s.end(), lt);
		cout << s[0].x << ' ' << s[0].y << ' ' << s[1].x << ' ' << s[1].y << '\n';
	}
}
