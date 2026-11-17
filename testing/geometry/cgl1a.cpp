#include "../../tex/contents/geometry/point.hpp"

// AOJ CGL_1_A: projection of p onto line p1p2
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	PD a, b;
	int q; cin >> a.x >> a.y >> b.x >> b.y >> q;
	while(q--){
		PD p; cin >> p.x >> p.y;
		PD r = lineProj(a, b, p);
		cout << r.x << ' ' << r.y << '\n';
	}
}
