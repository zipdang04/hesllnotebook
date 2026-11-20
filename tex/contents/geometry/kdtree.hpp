#pragma once
#include "point.hpp"

// kd-tree on integer points: nearest point to p. About
// O(log n) per query on random points, O(n) worst. Build
// O(n log n). Recursive, depth <= log2(n) + 2.
struct KdTree{
	struct Node{
		PL pt;
		ll x0, x1, y0, y1; // bounding box
		int l = 0, r = 0; // children; l == 0: leaf
	};
	vector<Node> t; // t[0] = root
	KdTree(vector<PL> pts){
		t.reserve(2 * sz(pts));
		build(0, sz(pts) - 1, pts);
	}
	int build(int lo, int hi, vector<PL> &pts){
		PL a = pts[lo]; Node nd{a, a.x, a.x, a.y, a.y};
		FOR(i, lo + 1, hi){
			minimize(nd.x0, pts[i].x); maximize(nd.x1, pts[i].x);
			minimize(nd.y0, pts[i].y); maximize(nd.y1, pts[i].y);
		}
		int id = sz(t); t.push_back(nd);
		if(lo == hi) return id;
		bool byX = nd.x1 - nd.x0 >= nd.y1 - nd.y0;
		int m = lo + (hi - lo + 1) / 2;
		nth_element(pts.begin() + lo, pts.begin() + m,
			pts.begin() + hi + 1, [&](PL u, PL v){
				return byX ? u.x < v.x : u.y < v.y;
			});
		int l = build(lo, m - 1, pts), r = build(m, hi, pts);
		t[id].l = l, t[id].r = r;
		return id;
	}
	// squared distance from p to the box of node id, 0 inside
	ll boxDist(int id, PL p){
		const Node &nd = t[id];
		ll x = min(max(nd.x0, p.x), nd.x1);
		ll y = min(max(nd.y0, p.y), nd.y1);
		return (PL{x, y} - p).dist2();
	}
	void search(int id, PL p, pair<ll, PL> &best){
		const Node &nd = t[id];
		if(!nd.l){
			// to skip the query point: if(nd.pt == p) return;
			best = min(best, {(nd.pt - p).dist2(), nd.pt});
			return;
		}
		int f = nd.l, s = nd.r;
		ll bf = boxDist(f, p), bs = boxDist(s, p);
		if(bf > bs) swap(bf, bs), swap(f, s);
		if(bf < best.fi) search(f, p, best);
		if(bs < best.fi) search(s, p, best);
	}
	pair<ll, PL> nearest(PL p){
		pair<ll, PL> best{LLONG_MAX, {}};
		search(0, p, best);
		return best;
	}
};
