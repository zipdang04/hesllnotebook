#pragma once
#include "point.hpp"

// n >= 2; duplicates allowed (distance 0)
pair<Point<ll>, Point<ll>> closestPair(vector<Point<ll>> v){
	typedef Point<ll> P;
	sort(v.begin(), v.end(), [](P a, P b){ return a.y < b.y; });
	set<P> s; // window |y - p.y| < d, ordered by x
	pair<ll, pair<P, P>> r{LLONG_MAX, {P(), P()}};
	int j = 0;
	for (P p: v){
		P d(1 + (ll)sqrtl(r.fi), 0);
		while (v[j].y <= p.y - d.x) s.erase(v[j++]);
		for (auto it = s.lower_bound(p - d), hi = s.upper_bound(p + d); it != hi; it++)
			r = min(r, {(*it - p).dist2(), {*it, p}});
		s.insert(p);
	}
	return r.se;
}
struct KDTree{
	typedef Point<ll> P;
	vector<pair<P, int>> t; // (point, input index), median-split on x / y alternately
	KDTree(const vector<P>& v){
		EFOR(int, i, 0, (int)v.size()) t.push_back({v[i], i});
		build(0, t.size(), 0);
	}
	void build(int l, int r, bool d){
		if (r - l < 2) return;
		int m = (l + r) / 2;
		nth_element(t.begin() + l, t.begin() + m, t.begin() + r, [d](auto& a, auto& b){
			return d ? a.fi.y < b.fi.y : a.fi.x < b.fi.x; });
		build(l, m, !d), build(m + 1, r, !d);
	}
	void go(int l, int r, bool d, P q, int skip, pair<ll, int>& best){
		if (l >= r) return;
		int m = (l + r) / 2;
		P p = t[m].fi;
		if (t[m].se != skip) best = min(best, {(p - q).dist2(), t[m].se});
		ll del = d ? q.y - p.y : q.x - p.x;
		bool s = del >= 0; // near side first, far side only if the splitting line is closer than best
		go(s ? m + 1 : l, s ? r : m, !d, q, skip, best);
		if (del * del < best.fi) go(s ? l : m + 1, s ? m : r, !d, q, skip, best);
	}
	// {dist^2, index} of the nearest input point other than index skip; {LLONG_MAX, -1} if none
	pair<ll, int> nearest(P q, int skip = -1){
		pair<ll, int> best{LLONG_MAX, -1};
		go(0, t.size(), 0, q, skip, best);
		return best;
	}
};
