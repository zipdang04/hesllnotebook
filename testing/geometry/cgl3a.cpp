#include "../../tex/contents/geometry/polygon.hpp"

int main(){
	int n; cin >> n;
	vector<Point<ll>> v(n);
	for (auto& p: v) cin >> p.x >> p.y;
	ll a = polyArea2(v);
	cout << fixed << setprecision(1) << a / 2.0L << '\n';
}
