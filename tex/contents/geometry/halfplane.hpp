#pragma once
#include "point.hpp"

// half-plane = left side of the directed line p -> p + d, d != 0
struct HalfPlane{
	Point<ld> p, d;
	bool out(Point<ld> q) const { return sgn(d.cross(q - p) / d.dist()) <= 0; } // not strictly inside
	Point<ld> inter(const HalfPlane& h) const { return p + d * (h.d.cross(p - h.p) / d.cross(h.d)); }
};
// intersection clipped to the box [-B, B]^2: ccw vertices, {} if its area is 0
vector<Point<ld>> halfPlaneInter(vector<HalfPlane> h, ld B = 1e6){
	typedef Point<ld> P;
	P c[4] = {P(B, B), P(-B, B), P(-B, -B), P(B, -B)};
	FOR(int, i, 0, 3) h.push_back({c[i], c[(i + 1) % 4] - c[i]});
	sort(h.begin(), h.end(), [](auto& a, auto& b){ return a.d.angle() < b.d.angle(); });
	deque<HalfPlane> q;
	for (auto& x: h){
		if (q.size() && !sgn(x.d.cross(q.back().d)) && x.d.dot(q.back().d) > 0){ // same direction: keep inner
			if (!x.out(q.back().p)) continue;
			q.pop_back();
		}
		while (q.size() > 1 && x.out(q.end()[-1].inter(q.end()[-2]))) q.pop_back();
		while (q.size() > 1 && x.out(q[0].inter(q[1]))) q.pop_front();
		if (q.size() && sgn(q.back().d.cross(x.d)) <= 0) return {};
		q.push_back(x);
	}
	while (q.size() > 2 && q[0].out(q.end()[-1].inter(q.end()[-2]))) q.pop_back();
	while (q.size() > 2 && q.back().out(q[0].inter(q[1]))) q.pop_front();
	if (q.size() < 3) return {};
	vector<P> r;
	FOR(int, i, 0, (int)q.size() - 1) r.push_back(q[i].inter(q[(i + 1) % q.size()]));
	return r;
}
