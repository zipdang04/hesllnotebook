#pragma once
#include "point.hpp"

// P = Point<ld>. Results with 1 point: tangency (decided with sgn, i.e. eps)
template<class P> vector<P> circleLine(P c, ld r, P a, P b){ // line ab, a != b
	P ab = b - a, p = a + ab * (c - a).dot(ab) / ab.dist2();
	ld s = a.cross(b, c), h2 = r * r - s * s / ab.dist2();
	if (sgn(h2) < 0) return {};
	if (!sgn(h2)) return {p};
	P h = ab.unit() * sqrtl(h2);
	return {p - h, p + h};
}
template<class P> vector<P> circleInter(P a, ld r1, P b, ld r2){ // {} if a == b
	P v = b - a;
	ld d2 = v.dist2(), p = (d2 + r1 * r1 - r2 * r2) / (2 * d2), h2 = r1 * r1 - p * p * d2;
	if (!sgn(d2) || sgn(h2) < 0) return {};
	P m = a + v * p, h = v.perp() * sqrtl(max((ld)0, h2) / d2);
	if (!sgn(h2)) return {m};
	return {m + h, m - h};
}
// external tangents (internal if r2 < 0), as {point on c1, point on c2}; point: r2 = 0.
// 0 if nested / overlapping (internal) / concentric, 1 if touching
template<class P> vector<pair<P, P>> tangents(P c1, ld r1, P c2, ld r2){
	P d = c2 - c1;
	ld dr = r1 - r2, d2 = d.dist2(), h2 = d2 - dr * dr;
	if (!sgn(d2) || sgn(h2) < 0) return {};
	vector<pair<P, P>> out;
	for (ld s: {-1, 1}){
		P v = (d * dr + d.perp() * sqrtl(max((ld)0, h2)) * s) / d2;
		out.push_back({c1 + v * r1, c2 + v * r2});
	}
	if (!sgn(h2)) out.pop_back();
	return out;
}
template<class P> P ccCenter(P a, P b, P c){ // circumcentre; a, b, c not collinear
	P u = c - a, v = b - a;
	return a + (u * v.dist2() - v * u.dist2()).perp() / u.cross(v) / 2;
}
template<class P> pair<P, ld> mec(vector<P> ps){ // minimum enclosing circle {centre, radius}
	shuffle(ps.begin(), ps.end(), mt19937(CURRENT_TIMESTAMP));
	P o = ps[0]; ld r = 0, E = 1 + 1e-9;
	EFOR(int, i, 0, (int)ps.size()) if ((o - ps[i]).dist() > r * E){
		o = ps[i], r = 0;
		EFOR(int, j, 0, i) if ((o - ps[j]).dist() > r * E){
			o = (ps[i] + ps[j]) / 2, r = (o - ps[i]).dist();
			EFOR(int, k, 0, j) if ((o - ps[k]).dist() > r * E)
				o = ccCenter(ps[i], ps[j], ps[k]), r = (o - ps[i]).dist();
		}
	}
	return {o, r};
}
