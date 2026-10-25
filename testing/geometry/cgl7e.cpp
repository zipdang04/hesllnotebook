#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
bool lt(P a, P b){ return abs(a.x - b.x) > 1e-9 ? a.x < b.x : a.y < b.y; }

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld v[6]; for (ld &t: v) cin >> t;
	auto s = circleInter(P(v[0], v[1]), v[2], P(v[3], v[4]), v[5]);
	if (s.size() == 1) s.push_back(s[0]);
	sort(s.begin(), s.end(), lt);
	cout << s[0].x << ' ' << s[0].y << ' ' << s[1].x << ' ' << s[1].y << '\n';
}
