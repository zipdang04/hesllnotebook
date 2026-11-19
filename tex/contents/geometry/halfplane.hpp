#pragma once
#include "point.hpp"

// Intersection of the half-planes "left of a->b",
// hs[i] = {a, b}. Counter-clockwise polygon, {} if empty.
// O(n log n). Clipped to the box |x|, |y| <= BOX.
vector<PD> halfPlaneInter(vector<pair<PD, PD>> hs){
	// above every coordinate of the answer, and at most
	// 1e6: rounding near a corner (BOX * 1e-16) < EPS
	const double BOX = 1e6;
	struct H{ PD p, pq; double ang; };
	vector<PD> bx = {{BOX, BOX}, {-BOX, BOX},
		{-BOX, -BOX}, {BOX, -BOX}};
	FOR(i, 0, 3) hs.push_back({bx[i], bx[(i+1)%4]});
	vector<H> b;
	for(auto [s, e]: hs){
		PD pq = e - s;
		// atan2 puts y = -0.0 at -pi, away from y = 0 at pi
		if(pq.y == 0) pq.y = 0;
		b.push_back({s, pq, atan2(pq.y, pq.x)});
	}
	sort(all(b), [](H u, H v){ return u.ang < v.ang; });
	int n = sz(b), q = 1, h = 0;
	vector<H> c(n + 10);
	// unit(): a distance, so EPS does not scale with BOX
	auto out = [](H u, PD r){
		return sgn(u.pq.unit().cross(r - u.p)) < 0;
	};
	auto inter = [](H u, H v){
		return u.p + u.pq * ((v.p - u.p).cross(v.pq)
			/ u.pq.cross(v.pq));
	};
	FOR(i, 0, n - 1){
		while(q < h && out(b[i], inter(c[h], c[h-1]))) h--;
		while(q < h && out(b[i], inter(c[q], c[q+1]))) q++;
		c[++h] = b[i];
		if(q < h && sgn(c[h].pq.cross(c[h-1].pq)) == 0){
			if(sgn(c[h].pq.dot(c[h-1].pq)) <= 0)
				return {};
			h--;
			if(out(b[i], c[h].p)) c[h] = b[i];
		}
	}
	while(q < h - 1 && out(c[q], inter(c[h], c[h-1]))) h--;
	while(q < h - 1 && out(c[h], inter(c[q], c[q+1]))) q++;
	if(h - q <= 1) return {};
	c[h+1] = c[q];
	vector<PD> s;
	PD last = inter(c[h], c[q]);
	FOR(i, q, h){
		PD v = inter(c[i], c[i+1]);
		// a corner on three lines would repeat
		if(sgn((v - last).dist())) s.push_back(v);
		last = v;
	}
	return s;
}
