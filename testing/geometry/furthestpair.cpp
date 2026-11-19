#include "../../tex/contents/geometry/convexhull.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int t;
	if(!(cin >> t))
		return 0;
	FOR(tc, 1, t){
		int n; cin >> n;
		vector<PL> pts(n);
		map<PL, vector<int>> at;
		FOR(i, 0, n - 1){
			cin >> pts[i].x >> pts[i].y;
			at[pts[i]].push_back(i);
		}
		auto [a, b] = hullDiameter(convexHull(pts));
		cout << at[a][0] << ' ' << at[b][a == b] << '\n';
	}
}
