#include "../../tex/contents/geometry/polygon.hpp"

int main(){
	int n, q; cin >> n;
	vector<Point<ll>> v(n);
	for (auto& p: v) cin >> p.x >> p.y;
	cin >> q;
	while (q--){
		Point<ll> p; cin >> p.x >> p.y;
		cout << inPolygon(v, p) + 1 << '\n';
	}
}
