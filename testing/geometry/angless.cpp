#include "../../tex/contents/geometry/point.hpp"
#include <cassert>

bool near(double a, double b){ return fabs(a - b) < 1e-12; }
bool near(PD a, PD b){
	return near(a.x, b.x) && near(a.y, b.y);
}
void selfCheck(){
	// aggregate, default zero
	PL z;
	assert(z.x == 0 && z.y == 0);
	vector<PL> w;
	w.push_back({1, 2});
	assert((PD(PL{3, 4}).dist() == 5));
	assert((PL{3, 4}.dist2() == 25));
	assert((PL{1, 0}.perp() == PL{0, 1}));
	assert((PL{1, 2} < PL{1, 3}));
	assert((PL{2, 3} * 2 == PL{4, 6}));
	ll C = 1000000000;
	PL p{-C, -C}, q{C, -C}, r{-C, C};
	assert(p.cross(q, r) == 4 * C * C);
	assert(orient(p, q, r) == 1);
	assert(sgn(0LL) == 0 && sgn(-5LL) == -1);
	assert(sgn(1e-10) == 0 && sgn(1e-8) == 1);
	assert(sgn(1e-9) == 0 && sgn(-2e-9) == -1);
	assert((orient(PD{0, 0}, PD{1, 0}, PD{0, 1}) == 1));
	PL o{0, 0}, e{2, 0};
	assert((near(lineDist(o, e, PL{0, 3}), 3)));
	assert((near(lineDist(o, e, PL{0, -3}), -3)));
	auto sq = lineInter({0, 0}, {2, 2}, {0, 2}, {2, 0});
	assert((sq.fi == 1 && near(sq.se, PD{1, 1})));
	PD u{0, 0}, v{1, 0};
	auto par = lineInter(u, v, {0, 1}, {1, 1});
	auto same = lineInter(u, v, {2, 0}, {3, 0});
	assert(par.fi == 0 && near(par.se, PD{0, 0}));
	assert(same.fi == -1 && near(same.se, PD{0, 0}));
	// PD instantiations
	PD d0{0, 0}, d2{2, 0};
	assert(onSegment(d0, d2, PD{1, 0}));
	assert(!onSegment(d0, d2, PD{1, 0.5}));
	assert(segInter(d0, d2, PD{1, -1}, PD{1, 1}));
	assert(!segInter(d0, d2, PD{1, 1}, PD{3, 1}));
	assert(near(lineDist(d0, d2, PD{5, 2}), 2));
	assert(near(lineDist(d0, d2, PD{5, -2}), -2));
	assert((segInter(o, e, PL{1, 0}, PL{3, 0})));
	assert((!segInter(o, e, PL{3, 0}, PL{4, 0})));
	assert((segInter(o, e, PL{2, 0}, PL{2, 5})));
	assert((segInter(PL{1, 0}, PL{1, 0}, o, e)));
	PL f{1, 1};
	assert(!segInter(f, f, o, e));
	assert(onSegment(o, e, o));
	assert((!onSegment(o, e, PL{3, 0})));
	PD a{1, 2}, b{4, 6};
	assert(near(segDist(a, a, b), (b - a).dist()));
	assert((near(PD{1, 0}.rotate(acos(-1) / 2), PD{0, 1})));
	assert((near(PD{3, 4}.unit(), PD{0.6, 0.8})));
	PD l0{0, 0}, l1{2, 0}, pt{1, 5};
	assert((near(lineProj(l0, l1, pt, true), PD{1, -5})));
	assert((near(lineProj(l0, l1, pt), PD{1, 0})));
	vector<PL> ring = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
		{1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
	FOR(i, 0, sz(ring) - 2)
		assert(angLess(ring[i], ring[i + 1]));
	PL x5{5, 0};
	assert(!angLess(o, x5) && !angLess(x5, o));
	assert(!angLess(ring[0], ring[0]));
}

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	selfCheck();
	int n; cin >> n;
	vector<PL> v(n);
	for(PL &p: v) cin >> p.x >> p.y;
	sort(all(v), angLess);
	for(PL p: v) cout << p.x << ' ' << p.y << '\n';
}
