#pragma once
#include "point.hpp"

template<class P> auto polyArea2(const vector<P>& v){ // twice the signed area, > 0 if ccw
	auto a = v.back().cross(v[0]);
	FOR(int, i, 0, (int)v.size() - 2) a += v[i].cross(v[i + 1]);
	return a;
}
template<class P> int inPolygon(const vector<P>& v, P a){ // 1 inside, 0 on boundary, -1 outside
	int c = 0, n = v.size();
	FOR(int, i, 0, n - 1){
		P p = v[i], q = v[(i + 1) % n];
		if (onSegment(p, q, a)) return 0;
		c ^= ((a.y < p.y) - (a.y < q.y)) * a.cross(p, q) > 0;
	}
	return c ? 1 : -1;
}
template<class P> vector<P> polygonCut(const vector<P>& v, P s, P e){ // part left of s->e
	vector<P> r;
	FOR(int, i, 0, (int)v.size() - 1){
		P cur = v[i], prev = i ? v[i - 1] : v.back();
		auto a = s.cross(e, cur), b = s.cross(e, prev);
		if ((a > 0) != (b > 0)) r.push_back(cur + (prev - cur) * (a / (a - b)));
		if (a > 0) r.push_back(cur);
	}
	return r;
}
