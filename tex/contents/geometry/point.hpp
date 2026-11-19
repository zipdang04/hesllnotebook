#pragma once
#include "../contest/template.hpp"

// 2D points. PL = exact, |x|, |y| <= 1e9; PD = double.
// Orientation tests use sgn: exact for ll, EPS for
// double. "Left of a->b" is the positive side everywhere.
// Sign of v; the double version takes |v| <= EPS as 0.
int sgn(ll v){ return (v > 0) - (v < 0); }
int sgn(double v){
	const double EPS = 1e-9; return (v > EPS) - (v < -EPS);
}
template<class T>
struct Point{
	typedef Point P;
	T x = 0, y = 0;
	template<class U>
	explicit operator Point<U>() const{ return {(U)x, (U)y}; }
	bool operator<(P p) const{
		return tie(x, y) < tie(p.x, p.y);
	}
	bool operator==(P p) const{
		return tie(x, y) == tie(p.x, p.y);
	}
	P operator+(P p) const{ return {x + p.x, y + p.y}; }
	P operator-(P p) const{ return {x - p.x, y - p.y}; }
	P operator*(T d) const{ return {x * d, y * d}; }
	P operator/(T d) const{ return {x / d, y / d}; }
	T dot(P p) const{ return x * p.x + y * p.y; }
	T cross(P p) const{ return x * p.y - y * p.x; }
	// (a - *this) x (b - *this)
	T cross(P a, P b) const{
		return (a - *this).cross(b - *this);
	}
	T dist2() const{ return x * x + y * y; }
	double dist() const{ return sqrt((double)dist2()); }
	// perp: rotated +90 degrees
	P perp() const{ return {-y, x}; }
	P unit() const{ return *this / dist(); }
	// PD only; rotate: t radians counter-clockwise
	P rotate(double t) const{
		return {x * cos(t) - y * sin(t),
			x * sin(t) + y * cos(t)};
	}
};
typedef Point<ll> PL;
typedef Point<double> PD;
// +1: c is left of a->b, 0: collinear, -1: right.
template<class T>
int orient(Point<T> a, Point<T> b, Point<T> c){
	return sgn(a.cross(b, c));
}
// p on segment ab, end points included.
template<class T>
bool onSegment(Point<T> a, Point<T> b, Point<T> p){
	if(a == b) return sgn((p - a).dist()) == 0;
	return sgn(p.cross(a, b)) == 0
		&& sgn((a - p).dot(b - p)) <= 0;
}
// Segments ab and cd share a point (touching counts).
template<class T> bool segInter(Point<T> a,
		Point<T> b, Point<T> c, Point<T> d){
	int oa = orient(c, d, a), ob = orient(c, d, b);
	int oc = orient(a, b, c), od = orient(a, b, d);
	if(oa * ob < 0 && oc * od < 0) return true;
	return onSegment(c, d, a) || onSegment(c, d, b)
		|| onSegment(a, b, c) || onSegment(a, b, d);
}
// PL inputs: w = (b - a).cross(d - c), q = c.cross(d, a),
// both ll; point = PD(a) + PD(b - a) * (q / (double)w).
// Lines ab, cd: {1, point}, {0, _} parallel, {-1, _} same.
pair<int, PD> lineInter(PD a, PD b, PD c, PD d){
	double w = (b - a).cross(d - c);
	if(sgn(w) == 0) return {-(sgn(a.cross(b, c)) == 0), {}};
	double p = c.cross(b, d), q = c.cross(d, a);
	return {1, (a * p + b * q) / w};
}
// Distance from p to segment ab.
double segDist(PD a, PD b, PD p){
	if(a == b) return (p - a).dist();
	double d = (b - a).dist2();
	double t = min(d, max((double)0, (p - a).dot(b - a)));
	return ((p - a) * d - (b - a) * t).dist() / d;
}
// Signed distance from p to line ab, positive on the left.
template<class T>
double lineDist(Point<T> a, Point<T> b, Point<T> p){
	return (double)(b - a).cross(p - a) / (b - a).dist();
}
// Foot of p on line ab; refl: the mirror image instead.
PD lineProj(PD a, PD b, PD p, bool refl = false){
	PD v = b - a; return p - v.perp() * (1 + refl)
		* v.cross(p - a) / v.dist2();
}
// a before b by angle in (-pi, pi]; (0, 0) has angle 0.
bool angLess(PL a, PL b){
	auto half = [](PL p){
		if(p.y < 0) return 0;
		return p.y == 0 && p.x >= 0 ? 1 : 2;
	};
	if(half(a) != half(b)) return half(a) < half(b);
	return a.cross(b) > 0;
}
