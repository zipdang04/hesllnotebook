#include "../../tex/contents/geometry/polygon.hpp"
#include <cassert>

void selfCheck(){
	assert(polygonArea2(vector<PL>{}) == 0);
	assert(polygonArea2(vector<PL>{{1, 2}}) == 0);
	assert(polygonArea2(vector<PL>{{1, 2}, {3, 4}}) == 0);
	vector<PL> v{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
	assert(polygonArea2(v) == 32);
	reverse(all(v));
	assert(polygonArea2(v) == -32);
	reverse(all(v));
	v.push_back(v.back());
	assert(polygonArea2(v) == 32);
	assert(inPolygon(v, PL{2, 2}));
	assert(!inPolygon(v, PL{0, 4}));
	assert(inPolygon(v, PL{0, 4}, false));
	vector<PL> u{{0, 0}, {6, 0}, {6, 6}, {4, 6},
		{4, 2}, {2, 2}, {2, 6}, {0, 6}};
	FOR(pass, 0, 1){
		assert(!inPolygon(u, PL{3, 4}));
		assert(!inPolygon(u, PL{3, 4}, false));
		for(PL p: vector<PL>{{4, 4}, {6, 6}}){
			assert(!inPolygon(u, p));
			assert(inPolygon(u, p, false));
		}
		for(PL p: vector<PL>{{1, 1}, {3, 1}}){
			assert(inPolygon(u, p));
			assert(inPolygon(u, p, false));
		}
		vector<PD> d;
		for(PL p: u) d.push_back(PD(p));
		assert(!inPolygon(d, PD{3, 4}));
		assert(inPolygon(d, PD{1, 1}));
		reverse(all(u));
	}
	vector<PD> d{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
	auto cut = polygonCut(d, PD{2, -1}, PD{2, 5});
	assert(polygonArea2(cut) == 16);
	cut = polygonCut(d, PD{5, -1}, PD{5, 5});
	assert(polygonArea2(cut) == 32);
	cut = polygonCut(d, PD{-1, -1}, PD{-1, 5});
	assert(cut.empty());
	d = {{0, 0}, {2, -2}, {4, 0}, {2, 2}};
	cut = polygonCut(d, PD{-1, -2}, PD{5, -2});
	assert(sz(cut) == 5);
	assert(polygonArea2(cut) == 16);
	assert(polygonArea2(cut) == polygonArea2(d));
}

// AOJ CGL_3_A: polygon area
int main(){
	selfCheck();
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n; cin >> n;
	vector<PL> v(n);
	for(PL &p: v) cin >> p.x >> p.y;
	cout << fixed << setprecision(1);
	cout << abs(polygonArea2(v)) / 2.0 << '\n';
}
