#include "../../tex/contents/geometry/halfplane.hpp"
#include <cassert>

double area2(const vector<PD> &v){
	double ans = 0;
	FOR(i, 0, sz(v) - 1)
		ans += v[i].cross(v[(i+1)%sz(v)]);
	return ans;
}

int main(){
	const double B = 1e6;
	auto close = [](double a, double b){
		return abs(a - b) <= 1e-6 * abs(b);
	};
	auto p = halfPlaneInter({});
	assert(sz(p) == 4 && close(area2(p), 8 * B * B));
	vector<pair<PD, PD>> hs = {
		{{0, 0}, {1, 0}}, {{1, 0}, {1, 1}},
		{{1, 1}, {0, 1}}, {{0, 1}, {0, 0}}};
	p = halfPlaneInter(hs);
	assert(sz(p) == 4 && close(area2(p), 2));
	for(PD v: p){
		assert(min(abs(v.x), abs(v.x - 1)) <= 1e-9);
		assert(min(abs(v.y), abs(v.y - 1)) <= 1e-9);
	}
	p = halfPlaneInter({hs[2], hs[0], hs[3], hs[2],
		hs[1], hs[3], hs[0], hs[1]});
	assert(close(area2(p), 2));
	assert(halfPlaneInter({{{0, 0}, {0, 1}},
		{{1, 1}, {1, 0}}}).empty());
	p = halfPlaneInter({{{0, 0}, {0, -1}},
		{{1, 0}, {1, 1}}});
	assert(close(area2(p), 4 * B));
	bool clipped = false;
	for(PD v: p)
		clipped |= abs(v.y) == B;
	assert(clipped);
	hs = {{{0, 0}, {1, 0}}, {{0, 5}, {1, 5}},
		{{0, 5}, {1, 5}}, {{1, 5}, {1, 6}},
		{{1, 6}, {0, 6}}, {{0, 6}, {0, 5}}};
	p = halfPlaneInter(hs);
	assert(close(area2(p), 2));
	hs[1] = {{0, 6}, {1, 6}};
	p = halfPlaneInter(hs);
	assert(abs(area2(p)) < 1e-6);
	p = halfPlaneInter({{{-9, -9}, {-20, -20}},
		{{9, -18}, {9, 6}}, {{10, 9}, {4, 9}}});
	assert(close(area2(p), (B + 9) * (B + 9)));
	p = halfPlaneInter({{{0, 0}, {-1, -1}}});
	assert(sz(p) == 3 && close(area2(p), 4 * B * B));
	p = halfPlaneInter({{{-0.1, -0.1}, {-1.1, -1.1}}});
	assert(sz(p) == 3 && close(area2(p), 4 * B * B));
	p = halfPlaneInter({{{1, 0}, {0, -0.0}},
		{{0, 0}, {-1, 0}}, {{1, 0}, {1, -1}}});
	assert(close(area2(p), 2 * (B - 1) * B));
	for(PD v: p)
		assert(v.x >= 1 - 1e-9 && v.y <= 1e-9);
	assert(halfPlaneInter({{{0, 0}, {0, -1}},
		{{0, 0}, {1, 0}}, {{0, -1}, {-1, 0}}}).empty());
	p = halfPlaneInter({{{0, 0}, {4, 0}},
		{{4, 0}, {0, 4}}, {{0, 4}, {0, 0}},
		{{10, 0}, {0, 10}}});
	assert(sz(p) == 3 && close(area2(p), 16));
	return 0;
}
