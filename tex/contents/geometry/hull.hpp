#pragma once
#include "point.hpp"

// ccw, starts at the lowest-x (then lowest-y) point; collinear and duplicate points dropped
template<class P> vector<P> convexHull(vector<P> pts){
	if (pts.size() <= 1) return pts;
	sort(pts.begin(), pts.end());
	vector<P> h(pts.size() + 1);
	int s = 0, t = 0;
	for (int it = 2; it--; s = --t, reverse(pts.begin(), pts.end()))
		for (P p: pts){
			while (t >= s + 2 && orient(h[t - 2], h[t - 1], p) <= 0) t--;
			h[t++] = p;
		}
	return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}
// farthest pair of a convexHull output (rotating calipers), O(n)
template<class P> array<P, 2> hullDiameter(const vector<P>& h){
	int n = h.size(), j = n < 2 ? 0 : 1;
	array<P, 2> r{h[0], h[0]};
	FOR(int, i, 0, j - 1) for (;; j = (j + 1) % n){
		if ((h[i] - h[j]).dist2() > (r[0] - r[1]).dist2()) r = {h[i], h[j]};
		if (sgn((h[(j + 1) % n] - h[j]).cross(h[i + 1] - h[i])) >= 0) break;
	}
	return r;
}
// 1 strictly inside, 0 on the boundary, -1 outside; h = non-empty convexHull output, O(log n)
template<class P> int inHull(const vector<P>& h, P p){
	int n = h.size(), lo = 1, hi = n - 1;
	if (n < 3) return onSegment(h[0], h[hi], p) - 1;
	int a = orient(h[0], h[1], p), b = orient(h[0], h[hi], p);
	if (a < 0 || b > 0) return -1;
	while (hi - lo > 1){
		int m = (lo + hi) / 2;
		(orient(h[0], h[m], p) >= 0 ? lo : hi) = m;
	}
	int s = orient(h[lo], h[hi], p);
	return s <= 0 ? s : a && b;
}
