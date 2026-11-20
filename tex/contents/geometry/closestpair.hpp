#pragma once
#include "point.hpp"

// The two closest points, O(n log n).
pair<PL, PL> closestPair(vector<PL> pts){
	set<PL> s;
	sort(all(pts), [](PL a, PL b){ return a.y < b.y; });
	pair<ll, pair<PL, PL>> ret{LLONG_MAX, {}};
	int j = 0;
	for(PL p: pts){
		PL d{1 + (ll)sqrt(ret.fi), 0};
		while(pts[j].y <= p.y - d.x) s.erase(pts[j++]);
		auto lo = s.lower_bound(p - d);
		auto hi = s.upper_bound(p + d);
		for(; lo != hi; ++lo)
			ret = min(ret, {(*lo - p).dist2(), {*lo, p}});
		s.insert(p);
	}
	return ret.se;
}
