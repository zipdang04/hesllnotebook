#include "../../tex/contents/geometry/circle.hpp"

// AOJ CGL_7_F: tangent points from a point to a circle
bool lessXY(PD a, PD b){
	if(abs(a.x - b.x) > 1e-9)
		return a.x < b.x;
	return a.y < b.y;
}
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	PD p, c; double r;
	cin >> p.x >> p.y >> c.x >> c.y >> r;
	vector<PD> v;
	for(auto [a, b]: circleTangents(c, r, p, 0))
		v.push_back(a);
	sort(all(v), lessXY);
	cout << fixed << setprecision(10);
	for(PD a: v)
		cout << a.x << ' ' << a.y << '\n';
}
