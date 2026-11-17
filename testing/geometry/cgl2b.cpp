#include "../../tex/contents/geometry/point.hpp"

// AOJ CGL_2_B: do two segments intersect
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int q; cin >> q;
	while(q--){
		PL a, b, c, d;
		cin >> a.x >> a.y >> b.x >> b.y;
		cin >> c.x >> c.y >> d.x >> d.y;
		cout << segInter(a, b, c, d) << '\n';
	}
}
