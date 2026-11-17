#include "../../tex/contents/geometry/point.hpp"

// AOJ CGL_1_C: counter clockwise
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	PL a, b;
	int q; cin >> a.x >> a.y >> b.x >> b.y >> q;
	while(q--){
		PL p; cin >> p.x >> p.y;
		int o = orient(a, b, p);
		if(o > 0) cout << "COUNTER_CLOCKWISE\n";
		else if(o < 0) cout << "CLOCKWISE\n";
		else if((b - a).dot(p - a) < 0) cout << "ONLINE_BACK\n";
		else if(onSegment(a, b, p)) cout << "ON_SEGMENT\n";
		else cout << "ONLINE_FRONT\n";
	}
}
