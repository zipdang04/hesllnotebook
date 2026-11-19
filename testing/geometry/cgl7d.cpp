#include "../../tex/contents/geometry/circle.hpp"

// AOJ CGL_7_D: intersection of a circle and a line
bool lessXY(PD a, PD b){
	if(abs(a.x - b.x) > 1e-9)
		return a.x < b.x;
	return a.y < b.y;
}
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	PD c; double r; int q;
	cin >> c.x >> c.y >> r >> q;
	while(q--){
		PD a, b; cin >> a.x >> a.y >> b.x >> b.y;
		auto v = circleLine(c, r, a, b);
		if(sz(v) == 1)
			v.push_back(v[0]);
		sort(all(v), lessXY);
		cout << v[0].x << ' ' << v[0].y << ' '
			<< v[1].x << ' ' << v[1].y << '\n';
	}
}
