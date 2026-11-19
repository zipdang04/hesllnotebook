#include "../../tex/contents/geometry/circle.hpp"

// AOJ CGL_7_G: common tangent points on the first circle
bool lessXY(PD a, PD b){
	if(abs(a.x - b.x) > 1e-9)
		return a.x < b.x;
	return a.y < b.y;
}
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	PD c1, c2; double r1, r2;
	cin >> c1.x >> c1.y >> r1 >> c2.x >> c2.y >> r2;
	vector<PD> v;
	for(double sign: {-1, 1})
		for(auto [a, b]: circleTangents(c1, r1, c2, sign * r2))
			v.push_back(a);
	sort(all(v), lessXY);
	cout << fixed << setprecision(10);
	for(PD a: v)
		cout << a.x << ' ' << a.y << '\n';
}
