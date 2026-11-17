#include "../../tex/contents/geometry/point.hpp"

// AOJ CGL_2_C: cross point of two (intersecting) segments
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	int q; cin >> q;
	while(q--){
		PL a, b, c, d;
		cin >> a.x >> a.y >> b.x >> b.y;
		cin >> c.x >> c.y >> d.x >> d.y;
		PD r = lineInter(PD(a), PD(b), PD(c), PD(d)).se;
		cout << r.x << ' ' << r.y << '\n';
	}
}
