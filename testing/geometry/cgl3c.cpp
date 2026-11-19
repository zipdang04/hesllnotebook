#include "../../tex/contents/geometry/polygon.hpp"

// AOJ CGL_3_C: point in polygon
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n; cin >> n;
	vector<PL> v(n);
	for(PL &p: v) cin >> p.x >> p.y;
	int q; cin >> q;
	while(q--){
		PL p; cin >> p.x >> p.y;
		cout << (inPolygon(v, p) ? 2
			: inPolygon(v, p, false) ? 1 : 0) << '\n';
	}
}
