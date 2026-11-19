#include "../../tex/contents/geometry/convexhull.hpp"
#include <cassert>

__int128 crossRaw(PL a, PL b, PL p){
	return (__int128)(b.x - a.x) * (p.y - a.y)
		- (__int128)(b.y - a.y) * (p.x - a.x);
}

bool onSegmentRaw(PL a, PL b, PL p){
	return crossRaw(a, b, p) == 0
		&& min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
		&& min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

__int128 distRaw(PL a, PL b){
	__int128 x = a.x - b.x, y = a.y - b.y;
	return x * x + y * y;
}

bool bruteInHull(const vector<PL> &h, PL q, bool strict){
	int k = sz(h);
	if(k < 3)
		return !strict && onSegmentRaw(h[0], h.back(), q);
	FOR(i, 0, k - 1){
		__int128 side = crossRaw(h[i], h[(i+1)%k], q);
		if(side < 0 || (strict && side == 0)) return false;
	}
	return true;
}

void check(const vector<PL> &pts, vector<PL> queries,
		bool small){
	auto h = convexHull(pts);
	int n = sz(pts), k = sz(h);
	assert(1 <= k && k <= n);
	assert(h[0] == *min_element(all(pts)));
	FOR(i, 0, k - 1){
		assert(find(all(pts), h[i]) != pts.end());
		FOR(j, 0, i - 1) assert(!(h[i] == h[j]));
		if(k >= 3){
			assert(crossRaw(h[i], h[(i+1)%k],
				h[(i+2)%k]) > 0);
			for(PL p: pts)
				assert(crossRaw(h[i], h[(i+1)%k], p) >= 0);
		}
		PL sum = h[i] + h[(i+1)%k];
		if(sum.x % 2 == 0 && sum.y % 2 == 0)
			queries.push_back(sum / 2);
	}
	if(k == 1)
		for(PL p: pts) assert(p == h[0]);
	if(k == 2)
		for(PL p: pts) assert(onSegmentRaw(h[0], h[1], p));
	__int128 want = 0;
	for(PL a: pts)
		for(PL b: pts) want = max(want, distRaw(a, b));
	auto [a, b] = hullDiameter(h);
	assert(find(all(h), a) != h.end());
	assert(find(all(h), b) != h.end());
	assert(distRaw(a, b) == want);
	vector<PD> hd;
	if(small){
		vector<PD> pd;
		for(PL p: pts) pd.push_back(PD(p));
		hd = convexHull(pd);
		assert(sz(hd) == k);
		FOR(i, 0, k - 1) assert(hd[i] == PD(h[i]));
		auto [c, d] = hullDiameter(hd);
		assert((c - d).dist2() == (double)want);
	}
	for(PL q: queries){
		assert(inHull(h, q) == bruteInHull(h, q, true));
		for(bool strict: {false, true}){
			bool inside = bruteInHull(h, q, strict);
			assert(inHull(h, q, strict) == inside);
			if(small) assert(inHull(hd, PD(q), strict) == inside);
		}
	}
}

int main(){
	assert(convexHull(vector<PL>{}).empty());
	mt19937_64 rng(712367);
	for(ll C: {1LL, 2LL, 3LL, 4LL, 5LL, 6LL, 1000000000LL}){
		uniform_int_distribution<ll> coord(-C, C);
		uniform_int_distribution<ll> query(-C - 1, C + 1);
		uniform_int_distribution<int> size(1, C <= 6 ? 10 : 6);
		FOR(it, 0, 9999){
			int n = size(rng);
			vector<PL> pts;
			FOR(i, 0, n - 1){
				PL p{coord(rng), coord(rng)};
				if(it % 8 == 0) p = {C, -C};
				if(it % 8 == 1) p.y = C;
				if(it % 8 == 2) p.y = p.x;
				if(it % 8 == 3 && i) p = pts[0];
				if(it % 8 == 4){
					p.x = rng() % 2 ? C : -C;
					p.y = rng() % 2 ? C : -C;
				}
				pts.push_back(p);
			}
			vector<PL> queries = pts;
			FOR(i, 0, 7)
				queries.push_back({query(rng), query(rng)});
			check(pts, queries, C <= 6);
		}
	}
}
