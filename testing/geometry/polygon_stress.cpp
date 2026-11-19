#include "../../tex/contents/geometry/polygon.hpp"
#include <cassert>

ll rawCross(PL a, PL b, PL p){
	return (b.x - a.x) * (p.y - a.y)
		- (b.y - a.y) * (p.x - a.x);
}
// 0 outside, 1 border, 2 inside; exact winding count.
int bruteInside(const vector<PL> &v, PL p){
	int winding = 0;
	FOR(i, 0, sz(v) - 1){
		PL a = v[i], b = v[(i+1) % sz(v)];
		ll cross = rawCross(a, b, p);
		ll dot = (a.x - p.x) * (b.x - p.x)
			+ (a.y - p.y) * (b.y - p.y);
		if(cross == 0 && dot <= 0) return 1;
		if(a.y <= p.y && p.y < b.y && cross > 0)
			winding++;
		if(b.y <= p.y && p.y < a.y && cross < 0)
			winding--;
	}
	return winding ? 2 : 0;
}
vector<PL> randomPolygon(mt19937_64 &rng, int n, ll c){
	while(true){
		vector<PL> v;
		set<pll> directions;
		while(sz(v) < n){
			ll x = (ll)(rng() % (2 * c + 1)) - c;
			ll y = (ll)(rng() % (2 * c + 1)) - c;
			ll g = gcd(abs(x), abs(y));
			if(!g) continue;
			if(!directions.insert({x / g, y / g}).se)
				continue;
			v.push_back({x, y});
		}
		sort(all(v), angLess);
		bool ok = true;
		FOR(i, 0, n - 1)
			if(rawCross(PL{}, v[i], v[(i+1) % n]) <= 0)
				ok = false;
		if(ok) return v;
	}
}
// Split the star into triangles from the origin.
// Similar triangles give the area cut from each one.
double bruteCutArea(const vector<PL> &v, PL a, PL b){
	double res = 0;
	FOR(i, 0, sz(v) - 1){
		PL tri[3] = {{}, v[i], v[(i+1) % sz(v)]};
		double h[3];
		int cnt = 0;
		FOR(j, 0, 2){
			h[j] = rawCross(a, b, tri[j]);
			cnt += h[j] > 0;
		}
		double area = rawCross(tri[0], tri[1], tri[2]);
		if(cnt == 0) continue;
		if(cnt == 3){
			res += area;
			continue;
		}
		int j = 0;
		while((h[j] > 0) != (cnt == 1)) j++;
		double part = area;
		FOR(k, 0, 2) if(k != j)
			part *= h[j] / (h[j] - h[k]);
		res += cnt == 1 ? part : area - part;
	}
	return res;
}
void checkCut(const vector<PL> &v,
		const vector<PD> &d, PL a, PL b){
	auto left = polygonCut(d, PD(a), PD(b));
	auto right = polygonCut(d, PD(b), PD(a));
	double area = polygonArea2(left);
	assert(abs(area - bruteCutArea(v, a, b)) < 1e-9);
	assert(abs(area + polygonArea2(right)
		- polygonArea2(d)) < 1e-9);
	for(PD p: left)
		assert(PD(a).cross(PD(b), p) > -1e-9);
}
void checkSmall(const vector<PL> &v, ll c, int sign){
	vector<PD> d;
	for(PL p: v) d.push_back(PD(p));
	int inside = 0, border = 0;
	FOR(x, -c - 1, c + 1) FOR(y, -c - 1, c + 1){
		PL p{x, y};
		int want = bruteInside(v, p);
		inside += want == 2;
		border += want == 1;
		assert(inPolygon(v, p) == (want == 2));
		assert(inPolygon(v, p, false) == (want != 0));
		assert(inPolygon(d, PD(p)) == (want == 2));
		assert(inPolygon(d, PD(p), false) == (want != 0));
	}
	ll area = sign * (2LL * inside + border - 2);
	assert(polygonArea2(v) == area);
	assert(abs(polygonArea2(d) - area) < 1e-9);
}
int main(){
	vector<PD> sq = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}};
	assert(inPolygon(sq, PD{1e-5, 1e-5}));
	assert(!inPolygon(sq, PD{0.5, 1e-12}));
	assert(polygonArea2(vector<PL>{}) == 0);
	assert(polygonArea2(vector<PL>{{1, 2}}) == 0);
	assert(polygonArea2(vector<PL>{{1, 2}, {3, 4}}) == 0);
	mt19937_64 rng(52026);
	FOR(rep, 0, 9999){
		ll c = 2 + rng() % 5;
		int n = 3 + rng() % 7;
		auto v = randomPolygon(rng, n, c);
		int sign = rep % 2 ? -1 : 1;
		if(sign < 0) reverse(all(v));
		if(rep % 3 == 0) v.insert(v.begin(), v[0]);
		checkSmall(v, c, sign);
		vector<PD> d;
		for(PL p: v) d.push_back(PD(p));
		PL a, b;
		do{
			a = {(ll)(rng() % (2 * c + 3)) - c - 1,
				(ll)(rng() % (2 * c + 3)) - c - 1};
			b = {(ll)(rng() % (2 * c + 3)) - c - 1,
				(ll)(rng() % (2 * c + 3)) - c - 1};
		}while(a == b);
		if(rep % 4 == 0){
			a = v.back();
			b = v[1];
		}
		checkCut(v, d, a, b);
	}
	FOR(rep, 0, 999){
		auto v = randomPolygon(rng, 3, 1000000000);
		if(rep % 2) reverse(all(v));
		vector<PL> queries = v;
		queries.push_back({0, 0});
		FOR(i, 0, 2){
			PL a = v[i], b = v[(i+1) % 3];
			if((a.x + b.x) % 2 || (a.y + b.y) % 2)
				continue;
			queries.push_back({(a.x + b.x) / 2,
				(a.y + b.y) / 2});
		}
		FOR(i, 0, 19){
			ll x = (ll)(rng() % 2000000001) - 1000000000;
			ll y = (ll)(rng() % 2000000001) - 1000000000;
			queries.push_back({x, y});
		}
		for(PL p: queries){
			int want = bruteInside(v, p);
			assert(inPolygon(v, p) == (want == 2));
			assert(inPolygon(v, p, false) == (want != 0));
		}
	}
}
