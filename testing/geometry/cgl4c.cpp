#include "../../tex/contents/geometry/polygon.hpp"

// AOJ CGL_4_C: area to the left of a line
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	int n; cin >> n;
	vector<PD> v(n);
	for(PD &p: v){
		PL a; cin >> a.x >> a.y; p = PD(a);
	}
	int q; cin >> q;
	while(q--){
		PL a, b;
		cin >> a.x >> a.y >> b.x >> b.y;
		cout << polygonArea2(polygonCut(v, PD(a), PD(b))) / 2
			<< '\n';
	}
}
