#pragma once
#include "point.hpp"

// Circles (centre c, radius r), all in double. O(1)
// each; enclosingCircle expected O(n). Tangency is a
// length test through sgn and gives one point.
// Circle (c, r) and line ab: 0, 1 or 2 points, ordered
// in the direction a->b.
vector<PD> circleLine(PD c, double r, PD a, PD b){
	double d = abs(lineDist(a, b, c));
	if(sgn(d - r) > 0) return {};
	PD p = lineProj(a, b, c);
	if(sgn(d - r) == 0) return {p};
	PD h = (b - a).unit() * sqrt((r - d) * (r + d));
	return {p - h, p + h};
}
// Two circles: 0, 1 or 2 points, first the one left of
// c1->c2. Same centre: none.
vector<PD> circleInter(PD c1, double r1, PD c2, double r2){
	PD vec = c2 - c1;
	double d2 = vec.dist2(), d = sqrt(d2);
	int out = sgn(d - r1 - r2), in = sgn(abs(r1 - r2) - d);
	if(sgn(d) == 0 || out > 0 || in > 0) return {};
	double p = (d2 + (r1 - r2) * (r1 + r2)) / (d2 * 2);
	double h2 = r1 * r1 - p * p * d2;
	PD mid = c1 + vec * p;
	if(out == 0 || in == 0) return {mid};
	PD per = vec.perp() * sqrt(max((double)0, h2) / d2);
	return {mid + per, mid - per};
}
// Outer tangents: pairs {point on circle 1, on circle 2}.
// Inner tangents: pass -r2. Point c2: pass r2 = 0.
vector<pair<PD, PD>> circleTangents(
		PD c1, double r1, PD c2, double r2){
	PD d = c2 - c1;
	double dr = r1 - r2, d2 = d.dist2();
	int s = sgn(d.dist() - abs(dr));
	if(sgn(d.dist()) == 0 || s < 0) return {};
	double h = sqrt(max((double)0, d2 - dr * dr));
	vector<pair<PD, PD>> out;
	for(double sign: {-1, 1}){
		PD v = (d * dr + d.perp() * h * sign) / d2;
		out.push_back({c1 + v * r1, c2 + v * r2});
	}
	if(s == 0) out.pop_back();
	return out;
}
// Centre of the circle through a, b, c (not collinear).
PD circumcenter(PD a, PD b, PD c){
	PD u = c - a, v = b - a;
	return a + (u * v.dist2() - v * u.dist2()).perp()
		/ u.cross(v) / 2;
}
// Smallest enclosing circle {centre, radius}.
pair<PD, double> enclosingCircle(vector<PD> pts){
	static mt19937 rng(CURRENT_TIMESTAMP());
	shuffle(all(pts), rng);
	PD o = pts[0];
	double r = 0, EPS = 1 + 1e-8;
	FOR(i, 0, sz(pts) - 1) if((o - pts[i]).dist() > r * EPS){
		o = pts[i], r = 0;
		FOR(j, 0, i - 1) if((o - pts[j]).dist() > r * EPS){
			o = (pts[i] + pts[j]) / 2; r = (o - pts[i]).dist();
			FOR(k, 0, j - 1) if((o - pts[k]).dist() > r * EPS){
				o = circumcenter(pts[i], pts[j], pts[k]);
				r = (o - pts[i]).dist();
			}
		}
	}
	return {o, r};
}
