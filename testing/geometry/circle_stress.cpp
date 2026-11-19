#include "../../tex/contents/geometry/circle.hpp"
#include <cassert>

int pick(mt19937 &rng, int lo, int hi){
	return uniform_int_distribution<int>(lo, hi)(rng);
}
double bruteCircle(const vector<PD> &pts){
	ld best = numeric_limits<ld>::infinity();
	auto consider = [&](ld x, ld y, ld r){
		if(r >= best) return;
		for(PD p: pts)
			if(hypotl(p.x - x, p.y - y)
					> r * (1 + 1e-12L) + 1e-12L)
				return;
		best = r;
	};
	FOR(i, 0, sz(pts) - 1){
		FOR(j, i, sz(pts) - 1){
			ld x = ((ld)pts[i].x + pts[j].x) / 2;
			ld y = ((ld)pts[i].y + pts[j].y) / 2;
			consider(x, y, hypotl(x - pts[i].x,
				y - pts[i].y));
		}
		FOR(j, i + 1, sz(pts) - 1)
			FOR(k, j + 1, sz(pts) - 1){
				ld ax = (ld)pts[j].x - pts[i].x;
				ld ay = (ld)pts[j].y - pts[i].y;
				ld bx = (ld)pts[k].x - pts[i].x;
				ld by = (ld)pts[k].y - pts[i].y;
				ld det = 2 * (ax * by - ay * bx);
				if(det == 0) continue;
				ld u = ax * ax + ay * ay;
				ld v = bx * bx + by * by;
				ld x = (u * by - v * ay) / det;
				ld y = (ax * v - bx * u) / det;
				consider(pts[i].x + x, pts[i].y + y,
					hypotl(x, y));
			}
	}
	assert(isfinite(best));
	return (double)best;
}
void stressMec(mt19937 &rng){
	int ranges[] = {3, 10, 1000, 1000000, 1000000000};
	FOR(rep, 0, 19999){
		int R = ranges[(rep/4)%5];
		int n = pick(rng, 1, 12), shape = rep % 4;
		PD base{(double)pick(rng, -R, R),
			(double)pick(rng, -R, R)};
		PD dir;
		do{
			dir = {(double)pick(rng, -1, 1),
				(double)pick(rng, -1, 1)};
		}while(dir.dist2() == 0);
		vector<PD> pts;
		FOR(i, 1, n){
			PD p{(double)pick(rng, -R, R),
				(double)pick(rng, -R, R)};
			if(shape == 1) p = dir * pick(rng, -R, R);
			if(shape == 2) p = base;
			if(shape == 3){
				double t = 2 * acos(-1.0)
					* pick(rng, 0, 999999) / 1000000;
				p = {round(R * cos(t)), round(R * sin(t))};
			}
			pts.push_back(p);
		}
		double best = bruteCircle(pts);
		auto [o, r] = enclosingCircle(pts);
		assert(isfinite(r) && r >= 0);
		assert(abs(r - best) <= 1e-6 * max(1.0, best));
		for(PD p: pts)
			assert((p - o).dist()
				<= r * (1 + 1e-7) + 1e-9);
	}
}
void onCircle(PD p, PD c, double r){
	assert(abs((p - c).dist() - r) <= 1e-6);
}
void checkGeometry(PL c1, ll r1, PL c2, ll r2,
		PL a, PL b){
	ll d2 = (c2 - c1).dist2();
	ll sum = r1 + r2, dif = r1 - r2;
	int cnt = 2;
	if(d2 == sum * sum || d2 == dif * dif) cnt = 1;
	if(d2 == 0 || d2 > sum * sum || d2 < dif * dif)
		cnt = 0;
	PD u = (PD)c1, v = (PD)c2;
	auto inter = circleInter(u, r1, v, r2);
	assert(sz(inter) == cnt);
	for(PD p: inter){
		onCircle(p, u, r1);
		onCircle(p, v, r2);
	}
	if(sz(inter) == 2){
		assert(orient(u, v, inter[0]) > 0);
		assert(orient(u, v, inter[1]) < 0);
	}
	ll cross = (b - a).cross(c1 - a);
	ll lhs = cross * cross;
	ll rhs = r1 * r1 * (b - a).dist2();
	cnt = lhs > rhs ? 0 : (lhs == rhs ? 1 : 2);
	PD x = (PD)a, y = (PD)b, dir = y - x;
	auto line = circleLine(u, r1, x, y);
	assert(sz(line) == cnt);
	for(PD p: line){
		onCircle(p, u, r1);
		assert(abs(dir.cross(p - x)) / dir.dist() <= 1e-6);
	}
	if(sz(line) == 2)
		assert((line[1] - line[0]).dot(dir) > 0);
	for(ll r: {r2, -r2, 0LL}){
		ll delta = r1 - r;
		cnt = d2 == delta * delta ? 1 : 2;
		if(d2 == 0 || d2 < delta * delta) cnt = 0;
		auto tangents = circleTangents(u, r1, v, r);
		assert(sz(tangents) == cnt);
		for(auto [p, q]: tangents){
			onCircle(p, u, r1);
			onCircle(q, v, abs(r));
			PD edge = q - p;
			double scale = max(1.0, r1 * edge.dist());
			assert(abs((p - u).dot(edge)) <= 1e-6 * scale);
			scale = max(1.0, abs(r) * edge.dist());
			assert(abs((q - v).dot(edge)) <= 1e-6 * scale);
			scale = max(1.0, (double)abs(r1 * r));
			assert(abs((p - u).dot(q - v) - r1 * r)
				<= 1e-6 * scale);
			if(r == 0) assert(q == v);
		}
	}
	if((c2 - c1).cross(a - c1) == 0) return;
	PD o = circumcenter(u, v, x);
	double radius = (o - u).dist();
	double tol = 1e-6 * max(1.0, radius);
	assert(isfinite(radius));
	assert(abs((o - v).dist() - radius) <= tol);
	assert(abs((o - x).dist() - radius) <= tol);
}
int main(){
	double r1 = 999.99, r2 = r1 - 1e-8;
	auto ci = circleInter({0, 0}, r1, {2e-8, 0}, r2);
	assert(sz(ci) && abs(ci[0].x - 499.9953966451) < 1e-6);
	mt19937 rng(540054);
	stressMec(rng);
	FOR(rep, 0, 19999){
		PL c1{pick(rng, -1000, 1000),
			pick(rng, -1000, 1000)};
		PL c2{pick(rng, -1000, 1000),
			pick(rng, -1000, 1000)};
		ll r1 = pick(rng, 0, 1000);
		ll r2 = pick(rng, 0, 1000);
		if(rep % 17 == 0) r1 = 0;
		if(rep % 19 == 0) r2 = 0;
		if(rep % 5 == 1) c2 = c1;
		if(rep % 5 >= 2){
			ll d = pick(rng, 0, 2000);
			if(rep % 5 == 2) d = abs(r1 - r2);
			if(rep % 5 == 3) d = r1 + r2;
			c1 = {-1000, pick(rng, -1000, 1000)};
			c2 = {c1.x + d, c1.y};
		}
		PL a{pick(rng, -1000, 1000),
			pick(rng, -1000, 1000)};
		PL b{pick(rng, -1000, 1000),
			pick(rng, -1000, 1000)};
		if(rep % 3){
			ll x = c1.x;
			if(rep % 3 == 1) x += c1.x > 0 ? -r1 : r1;
			a = {x, -1000}; b = {x, 1000};
		}
		if(a == b) continue;
		if(rep % 2) swap(a, b);
		checkGeometry(c1, r1, c2, r2, a, b);
	}
	return 0;
}
