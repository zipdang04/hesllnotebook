#include "../../tex/contents/geometry/circle.hpp"

// AOJ CGL_7_C: circumcircle of a triangle
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	PD a, b, c;
	cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
	PD o = circumcenter(a, b, c);
	cout << fixed << setprecision(10);
	cout << o.x << ' ' << o.y << ' ' << (o - a).dist()
		<< '\n';
}
