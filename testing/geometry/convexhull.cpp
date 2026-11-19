#include "../../tex/contents/geometry/convexhull.hpp"
#include <cassert>

void selfCheck(){
	assert(convexHull(vector<PL>{}).empty());
	auto h = convexHull(vector<PL>{{1, 1}, {1, 1}, {1, 1}});
	assert((h == vector<PL>{{1, 1}}));
	h = convexHull(vector<PL>{
		{2, 2}, {0, 0}, {1, 1}, {3, 3}, {1, 1}});
	assert((h == vector<PL>{{0, 0}, {3, 3}}));
	vector<PL> pts{{0, 0}, {4, 0}, {4, 4}, {0, 4},
		{2, 2}, {2, 0}, {4, 4}};
	h = convexHull(pts);
	vector<PL> want{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
	assert(h == want);
	vector<PD> real;
	for(PL p: pts)
		real.push_back(PD(p));
	auto hd = convexHull(real);
	assert(sz(hd) == sz(want));
	FOR(i, 0, sz(hd) - 1)
		assert(hd[i] == PD(want[i]));
	auto [a, b] = hullDiameter(h);
	assert((a - b).dist2() == 32);
	auto [ad, bd] = hullDiameter(hd);
	assert((ad - bd).dist2() == 32);
	assert(inHull(hd, PD{2, 2}));
	assert(!inHull(hd, PD{2, 0}));
	assert(inHull(hd, PD{2, 0}, false));
	assert(inHull(h, PL{2, 2}));
	assert(!inHull(h, PL{2, 0}));
	assert(inHull(h, PL{2, 0}, false));
	assert(!inHull(h, PL{0, 0}));
	assert(inHull(h, PL{0, 0}, false));
	assert(!inHull(h, PL{5, 2}, false));
	h = convexHull(vector<PL>{{5, 5}});
	auto single = hullDiameter(h);
	assert((single.fi == PL{5, 5}));
	assert((single.se == PL{5, 5}));
	h = convexHull(vector<PL>{{1, 1}});
	assert(inHull(h, PL{1, 1}, false));
	assert(!inHull(h, PL{1, 1}));
	assert(!inHull(h, PL{1, 2}, false));
	h = convexHull(vector<PL>{{0, 0}, {4, 4}});
	assert(inHull(h, PL{2, 2}, false));
	assert(!inHull(h, PL{2, 2}));
	assert(!inHull(h, PL{5, 5}, false));
	assert(!inHull(h, PL{2, 3}, false));
	ll C = 1000000000;
	h = convexHull(vector<PL>{
		{-C, -C}, {C, -C}, {C, C}, {-C, C}});
	assert(sz(h) == 4);
	assert(inHull(h, PL{C - 1, C}, false));
	assert(!inHull(h, PL{C - 1, C}));
	assert(inHull(h, PL{0, 0}));
	assert(!inHull(h, PL{C, C + 1}, false));
	auto far = hullDiameter(h);
	assert((far.fi - far.se).dist2() == 8 * C * C);
}
int main(){
	selfCheck();
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int t;
	if(!(cin >> t))
		return 0;
	FOR(tc, 1, t){
		int n; cin >> n;
		vector<PL> pts(n);
		for(PL &p: pts)
			cin >> p.x >> p.y;
		auto h = convexHull(pts);
		cout << sz(h) << '\n';
		for(PL p: h)
			cout << p.x << ' ' << p.y << '\n';
	}
}
