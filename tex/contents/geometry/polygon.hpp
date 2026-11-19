#pragma once
#include "point.hpp"

// Polygons: vertices 0..n-1 in order, cyclic, either
// orientation. O(n) each.
// Twice the signed area; positive = counter-clockwise.
template<class T>
T polygonArea2(const vector<Point<T>> &v){
	T a = 0; int n = sz(v);
	FOR(i, 0, n - 1)
		a += v[i].cross(v[(i + 1) % n]);
	return a;
}
// p inside polygon v. strict: the border is outside.
template<class T> bool inPolygon(const vector<Point<T>> &v,
		Point<T> p, bool strict = true){
	int cnt = 0, n = sz(v);
	FOR(i, 0, n - 1){
		Point<T> q = v[(i + 1) % n];
		if(onSegment(v[i], q, p)) return !strict;
		cnt ^= ((p.y < v[i].y) - (p.y < q.y))
			* p.cross(v[i], q) > 0;
	}
	return cnt;
}
// The part of v on the left of a->b (v convex: one piece).
vector<PD> polygonCut(const vector<PD> &v, PD a, PD b){
	vector<PD> res;
	FOR(i, 0, sz(v) - 1){
		PD cur = v[i], prev = i ? v[i-1] : v.back();
		double ca = a.cross(b, cur), cb = a.cross(b, prev);
		if((ca > 0) != (cb > 0))
			res.push_back(cur + (prev - cur) * (ca / (ca - cb)));
		if(ca > 0)
			res.push_back(cur);
	}
	return res;
}
