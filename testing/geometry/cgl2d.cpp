#include "../../tex/contents/geometry/point.hpp"

// AOJ CGL_2_D: distance between two segments
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	int q; cin >> q;
	while(q--){
		PL a, b, c, d;
		cin >> a.x >> a.y >> b.x >> b.y;
		cin >> c.x >> c.y >> d.x >> d.y;
		double res = 0;
		if(!segInter(a, b, c, d)){
			PD pa(a), pb(b), pc(c), pd(d);
			res = min({segDist(pa, pb, pc), segDist(pa, pb, pd),
				segDist(pc, pd, pa), segDist(pc, pd, pb)});
		}
		cout << res << '\n';
	}
}
