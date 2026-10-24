#include "../../tex/contents/geometry/point.hpp"

typedef Point<ll> P; typedef Point<ld> Q;
mt19937 mt(12345);
int rnd(int a, int b) { return uniform_int_distribution<int>(a, b)(mt); }
Q toQ(P p) { return Q(p.x, p.y); }
ld ptSeg(Q s, Q e, Q p) { // clamped projection, independent of segDist
	Q v = e - s; ld L = v.dist2(), t = L == 0 ? 0 : max((ld)0, min((ld)1, (p - s).dot(v) / L));
	return (s + v * t - p).dist();
}
ld bruteDist(Q a, Q b, Q c, Q d) { // distance is convex in the parameter of ab
	ld lo = 0, hi = 1;
	auto f = [&](ld t) { return ptSeg(c, d, a + (b - a) * t); };
	for (int i = 0; i < 100; i++) {
		ld m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
		(f(m1) < f(m2) ? hi : lo) = f(m1) < f(m2) ? m2 : m1;
	}
	return f((lo + hi) / 2);
}
// exact expected intersection (sorted); integer coords, so everything but the crossing is exact
vector<Q> expected(P a, P b, P c, P d) {
	P dir = a == b ? d - c : b - a, base = a == b ? c : a;
	if (dir == P(0, 0)) return a == c ? vector<Q>{toQ(a)} : vector<Q>{};
	bool col = true;
	for (P p: {a, b, c, d}) col &= dir.cross(p - base) == 0;
	if (col) {
		ll lo = max(min(a.dot(dir), b.dot(dir)), min(c.dot(dir), d.dot(dir)));
		ll hi = min(max(a.dot(dir), b.dot(dir)), max(c.dot(dir), d.dot(dir)));
		vector<Q> r;
		for (ll u: {lo, hi}) for (P p: {a, b, c, d}) if (p.dot(dir) == u) { r.push_back(toQ(p)); break; }
		if (lo > hi) r.clear();
		if (lo == hi && !r.empty()) r.pop_back();
		sort(r.begin(), r.end());
		return r;
	}
	ll den = (b - a).cross(d - c);
	if (den == 0) return {};
	ll tn = (c - a).cross(d - c), sn = (c - a).cross(b - a); // a+(b-a)t = c+(d-c)s
	if (den < 0) den = -den, tn = -tn, sn = -sn;
	if (tn < 0 || tn > den || sn < 0 || sn > den) return {};
	return {toQ(a) + toQ(b - a) * ((ld)tn / den)};
}
bool near(Q p, Q q) { return (p - q).dist() < 1e-9; }
bool bad(const char* what, P a, P b, P c, P d) {
	printf("FAIL %s: a=(%lld,%lld) b=(%lld,%lld) c=(%lld,%lld) d=(%lld,%lld)\n", what, a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y);
	return true;
}
template<class T> bool checkInter(P a, P b, P c, P d, const vector<Q>& e, bool exact) {
	typedef Point<T> U;
	U A(a.x, a.y), B(b.x, b.y), C(c.x, c.y), D(d.x, d.y);
	auto v = segInter(A, B, C, D);
	if (v.size() != e.size()) return false;
	for (int i = 0; i < (int)v.size(); i++) {
		Q q(v[i].x, v[i].y);
		if (exact && !near(q, e[i])) return false;
		if (exact && !(onSegment(A, B, v[i]) && onSegment(C, D, v[i]))) return false;
	}
	return true;
}
template<class T> bool checkLine(P a, P b, P c, P d) {
	typedef Point<T> U;
	U A(a.x, a.y), B(b.x, b.y), C(c.x, c.y), D(d.x, d.y);
	auto [code, p] = lineInter(A, B, C, D);
	bool par = (b - a).cross(d - c) == 0, same = par && (b - a).cross(c - a) == 0;
	if (code != (par ? (same ? -1 : 0) : 1)) return false;
	if (code != 1 || is_same_v<T, ll>) return true;
	Q q(p.x, p.y), u = toQ(b - a), w = toQ(d - c);
	return fabsl(u.cross(q - toQ(a))) / u.dist() < 1e-9 && fabsl(w.cross(q - toQ(c))) / w.dist() < 1e-9;
}

int main() {
	for (int it = 0; it < 200000; it++) {
		int K = it % 3 == 0 ? 2 : 5;
		auto pt = [&] { return P(rnd(-K, K), rnd(-K, K)); };
		P a = pt(), b = pt(), c = pt(), d = pt();
		if (it % 7 == 0) b = a;
		if (it % 11 == 0) d = c;
		auto e = expected(a, b, c, d);
		bool crossing = e.size() == 1 && !(a == b || c == d) && (b - a).cross(d - c) != 0;
		if (!checkInter<ld>(a, b, c, d, e, true)) return bad("segInter ld", a, b, c, d);
		// ll: crossing point is truncated by integer division, so only check sizes there
		if (!checkInter<ll>(a, b, c, d, e, !crossing)) return bad("segInter ll", a, b, c, d);
		Q qa = toQ(a), qb = toQ(b), qc = toQ(c), qd = toQ(d);
		ld bd = bruteDist(qa, qb, qc, qd);
		if ((bd < 1e-9) != !e.empty()) return bad("empty vs brute", a, b, c, d);
		if (fabsl(segSegDist(a, b, c, d) - bd) > 1e-6 || fabsl(segSegDist(qa, qb, qc, qd) - bd) > 1e-6)
			return bad("segSegDist", a, b, c, d);
		if (fabsl(segDist(qa, qb, qc) - ptSeg(qa, qb, qc)) > 1e-6 || fabsl(segDist(a, b, c) - ptSeg(qa, qb, qc)) > 1e-6)
			return bad("segDist", a, b, c, d);
		if (!(a == b) && !(c == d)) {
			P e1 = b, s1 = a;
			if (!checkLine<ll>(s1, e1, c, d) || !checkLine<ld>(s1, e1, c, d)) return bad("lineInter", a, b, c, d);
		}
		if (!(a == P(0, 0))) {
			Q p = qa; ld ang = (rnd(-1000, 1000) / 1000.0L) * 2 * M_PI;
			Q r = p.rotate(ang);
			if (fabsl(remainderl(r.angle() - p.angle() - ang, 2 * M_PI)) > 1e-9 || fabsl(r.dist() - p.dist()) > 1e-9
				|| fabsl(p.unit().dist() - 1) > 1e-9) return bad("rotate/unit/angle", a, b, c, d);
		}
	}
}
