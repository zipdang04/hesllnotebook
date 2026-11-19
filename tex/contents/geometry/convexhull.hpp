#pragma once
#include "point.hpp"

// Convex hull (monotone chain), its diameter (calipers)
// and point-in-hull (binary search). Hull: vertices
// 0..k-1 cyclic, counter-clockwise, strictly convex.
// Hull with h[0] = smallest (x, y). O(n log n).
template<class T>
vector<Point<T>> convexHull(vector<Point<T>> pts){
	if(sz(pts) <= 1)
		return pts;
	sort(all(pts));
	vector<Point<T>> h(sz(pts) + 1);
	int s = 0, t = 0;
	// lower hull, then upper hull
	FOR(it, 1, 2){
		for(Point<T> p: pts){
			while(t >= s + 2 && orient(h[t-2], h[t-1], p) <= 0)
				t--;
			h[t++] = p;
		}
		s = --t; reverse(all(pts));
	}
	return {h.begin(),
		h.begin() + t - (t == 2 && h[0] == h[1])};
}
// The two farthest points of hull h, O(k).
template<class T> pair<Point<T>, Point<T>>
		hullDiameter(const vector<Point<T>> &h){
	int n = sz(h), j = n < 2 ? 0 : 1;
	pair<T, pair<Point<T>, Point<T>>> res{0, {h[0], h[0]}};
	for(int i = 0; i < j; i++)
		for(;; j = (j + 1) % n){
			maximize(res, make_pair((h[i] - h[j]).dist2(),
				make_pair(h[i], h[j])));
			Point<T> e = h[(j + 1) % n] - h[j];
			if(sgn(e.cross(h[i+1] - h[i])) >= 0)
				break;
		}
	return res.se;
}
// p inside hull h, O(log k). strict: the border is outside.
template<class T> bool inHull(const vector<Point<T>> &h,
		Point<T> p, bool strict = true){
	int a = 1, b = sz(h) - 1, r = !strict;
	if(sz(h) < 3)
		return r && onSegment(h[0], h.back(), p);
	if(orient(h[0], h[a], h[b]) > 0)
		swap(a, b);
	if(orient(h[0], h[a], p) >= r
		|| orient(h[0], h[b], p) <= -r)
		return false;
	while(abs(a - b) > 1){
		int c = (a + b) / 2;
		(orient(h[0], h[c], p) > 0 ? b : a) = c;
	}
	return orient(h[a], h[b], p) < r;
}
