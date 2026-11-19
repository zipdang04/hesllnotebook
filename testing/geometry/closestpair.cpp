#include "../../tex/contents/geometry/closestpair.hpp"
#include <cassert>

int main(){
	PL p{3, 4};
	auto [a, b] = closestPair({p, p});
	assert(a == p && b == p);
	ll C = 1000000000;
	tie(a, b) = closestPair({{-C, -C}, {C, C}});
	assert((a - b).dist2() == 8000000000000000000LL);
	tie(a, b) = closestPair({{0, 0}, p, {8, 9}, p, {6, 7}});
	assert(a == p && b == p);
	tie(a, b) = closestPair({{7, 0}, {7, 10},
		{7, 13}, {7, 30}});
	PL u{7, 10}, v{7, 13};
	assert((a == u && b == v) || (a == v && b == u));
	ios::sync_with_stdio(0); cin.tie(0);
	int T; cin >> T;
	while(T--){
		int n; cin >> n;
		vector<PL> pts(n);
		for(PL &q: pts)
			cin >> q.x >> q.y;
		tie(a, b) = closestPair(pts);
		int i = 0, j = 0;
		while(!(pts[i] == a))
			i++;
		while(j == i || !(pts[j] == b))
			j++;
		cout << i << ' ' << j << '\n';
	}
}
