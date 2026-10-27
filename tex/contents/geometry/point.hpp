#pragma once
#include "../contest/template.hpp"

const ld eps = 1e-9;
template<class T> int sgn(T x) { return (x > eps) - (x < -eps); }
template<class T> struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x = 0, T y = 0): x(x), y(y) {}
	bool operator<(P p) const { return tie(x, y) < tie(p.x, p.y); }
	bool operator==(P p) const { return tie(x, y) == tie(p.x, p.y); }
	P operator+(P p) const { return P(x + p.x, y + p.y); }
	P operator-(P p) const { return P(x - p.x, y - p.y); }
	P operator*(T d) const { return P(x * d, y * d); }
	P operator/(T d) const { return P(x / d, y / d); }
	T dot(P p) const { return x * p.x + y * p.y; }
	T cross(P p) const { return x * p.y - y * p.x; }
	T cross(P a, P b) const { return (a - *this).cross(b - *this); } // > 0 iff *this, a, b turn left
	T dist2() const { return x * x + y * y; }
	ld dist() const { return sqrtl(dist2()); }
	ld angle() const { return atan2l(y, x); }
	P unit() const { return *this / dist(); }
	P perp() const { return P(-y, x); }                      // rotated +90 deg
	P rotate(ld a) const { return P(x * cosl(a) - y * sinl(a), x * sinl(a) + y * cosl(a)); } // ccw
};
template<class P> int orient(P a, P b, P c) { return sgn(a.cross(b, c)); } // 1 if c left of a->b
template<class P> bool onSegment(P s, P e, P p) { return !sgn(p.cross(s, e)) && sgn((s - p).dot(e - p)) <= 0; }
template<class P> P lineProj(P a, P b, P p, bool refl = false) { // refl: reflection instead
	P v = b - a;
	return p - v.perp() * (1 + refl) * v.cross(p - a) / v.dist2();
}
// {1, point} unique, {0, .} parallel, {-1, .} same line
template<class P> pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (sgn(d) == 0) return {-(sgn(s1.cross(e1, s2)) == 0), P(0, 0)};
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return {1, (s1 * p + e1 * q) / d};
}
// {} none, {pt} one point, {p, q} overlap endpoints (sorted)
template<class P> vector<P> segInter(P a, P b, P c, P d) {
	auto oa = c.cross(d, a), ob = c.cross(d, b), oc = a.cross(b, c), od = a.cross(b, d);
	if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) return {(a * ob - b * oa) / (ob - oa)};
	set<P> s;
	for (P p: {a, b}) if (onSegment(c, d, p)) s.insert(p);
	for (P p: {c, d}) if (onSegment(a, b, p)) s.insert(p);
	return {s.begin(), s.end()};
}
template<class P> ld segDist(P s, P e, P p) { // s == e allowed
	if ((p - s).dot(e - s) <= 0) return (p - s).dist();
	if ((p - e).dot(s - e) <= 0) return (p - e).dist();
	return abs((ld)(e - s).cross(p - s)) / (e - s).dist();
}
template<class P> ld segSegDist(P a, P b, P c, P d) {
	if (!segInter(a, b, c, d).empty()) return 0;
	return min({segDist(a, b, c), segDist(a, b, d), segDist(c, d, a), segDist(c, d, b)});
}
