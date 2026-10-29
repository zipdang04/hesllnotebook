#include "../../tex/contents/geometry/halfplane.hpp"
typedef Point<ld> P; typedef __int128 L; typedef Point<ll> I;
mt19937 mt(1);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
ld area(const vector<P>& v){ ld s = 0; FOR(int, i, 0, (int)v.size() - 1) s += v[i].cross(v[(i+1)%v.size()]); return s/2; }
L cr(L ax, L ay, L bx, L by){ return ax*by - ay*bx; }
// exact: all pairwise line intersections inside every half-plane, hull area
ld brute(vector<pair<I,I>> h, ll B, bool& degen){
	I c[4] = {I(B,B), I(-B,B), I(-B,-B), I(B,-B)};
	FOR(int, i, 0, 3) h.push_back({c[i], I(c[(i+1)%4].x - c[i].x, c[(i+1)%4].y - c[i].y)});
	int n = h.size(); vector<P> pts; set<array<L,3>> seen;
	FOR(int, i, 0, n - 1) FOR(int, j, i+1, n - 1){
		auto [pi, di] = h[i]; auto [pj, dj] = h[j];
		L D = cr(di.x, di.y, dj.x, dj.y); if (!D) continue;
		L T = cr(dj.x, dj.y, pi.x - pj.x, pi.y - pj.y);
		L X = (L)pi.x*D + (L)di.x*T, Y = (L)pi.y*D + (L)di.y*T;
		bool ok = true;
		for (auto [pk, dk]: h){ L s = cr(dk.x, dk.y, X - (L)pk.x*D, Y - (L)pk.y*D); if (D < 0) s = -s; if (s < 0){ ok = false; break; } }
		if (!ok) continue;
		if (D < 0) D = -D, X = -X, Y = -Y;
			pts.push_back(P((ld)X / (ld)D, (ld)Y / (ld)D));
	}
	sort(pts.begin(), pts.end()); pts.erase(unique(pts.begin(), pts.end()), pts.end());
	if (pts.size() < 3){ degen = true; return 0; }
	vector<P> hull(2*pts.size()); int k = 0;
	for (int it = 0; it < 2; it++, reverse(pts.begin(), pts.end())){ int s = k;
		for (P p: pts){ while (k >= s + 2 && hull[k-2].cross(hull[k-1], p) <= 0) k--; hull[k++] = p; } k--; }
	hull.resize(k); ld a = k < 3 ? 0 : area(hull);
	degen = a < 1e-12L*B*B; return a;
}
int main(){ ll CC = 10000, BB = 1000000; mt.seed(1);
	int T = 300000; long long fails=0, degen=0, unb=0;
	FOR(int, it, 0, T - 1){
		int mode = it % 4; ll C = mode==0 ? 3 : mode==1 ? 10 : mode==2 ? 1000 : CC; ll B = mode==3 ? BB : 10*C;
		int n = rnd(0, mode==0 ? 6 : 10);
		vector<pair<I,I>> h;
		FOR(int, i, 0, n - 1){ I a(rnd(-C,C), rnd(-C,C)), b; do b = I(rnd(-C,C), rnd(-C,C)); while (b == a);
			h.push_back({a, b - a}); if (rnd(0,4)==0) h.push_back(h[rnd(0,(int)h.size()-1)]);
			int t = rnd(0,7); auto x = h.back();
			if (t==0) h.push_back({x.fi + x.se*2, x.se*-1});           // same line reversed
			if (t==1) h.push_back({x.fi + x.se.perp(), x.se*-1});      // reversed, shifted (strip/empty)
			if (t==2) h.push_back({x.fi + x.se.perp()*-1, x.se*-1});
			if (t==3) h.push_back({x.fi + x.se.perp(), x.se*3});       // parallel same dir
		}
		shuffle(h.begin(), h.end(), mt);
		bool dg = false; ld ea = brute(h, B, dg);
		vector<HalfPlane> hh; for (auto [p, d]: h) hh.push_back({P(p.x,p.y), P(d.x,d.y)});
		auto r = halfPlaneInter(hh, B);
		ld ra = r.empty() ? 0 : area(r);
		bool ok = fabsl(ea - ra) <= 1e-9 * max((ld)1, ea) + 1e-12L*B*B && (r.empty() || r.size() >= 3);
		FOR(int, i, 0, (int)r.size() - 1) if (orient(r[i], r[(i+1)%r.size()], r[(i+2)%r.size()]) <= 0) ok = false;
		for (auto& q: r) for (auto& x: hh) if (x.d.cross(q - x.p) / x.d.dist() < -1e-6) ok = false;
		if (ea > 0.9L*4*B*B) unb++;
		if (dg) degen++;
		if (!dg && r.empty()) ok = false;
		if (!ok){
			printf("FAIL it=%d expected area %.9Lf got %.9Lf (%zu vertices), B=%lld\n", it, ea, ra, r.size(), B);
			for (auto& x: h) printf("  p=(%lld,%lld) d=(%lld,%lld)\n", x.fi.x, x.fi.y, x.se.x, x.se.y);
			for (auto& q: r) printf("  r (%.9Lf,%.9Lf)\n", q.x, q.y);
			return 1;
		}
	}
	printf("cases=%d degenerate=%lld unbounded=%lld\n", T, degen, unb);
	{
	// convex lattice polygon: edges = primitive vectors sorted by angle
	vector<Point<ll>> e; int K = 40;
	FOR(int,a,-K,K) FOR(int,b,-K,K) if (gcd(abs(a),abs(b))==1) e.push_back(Point<ll>(a,b));
	sort(e.begin(), e.end(), [](auto u, auto v){ return atan2l(u.y,u.x) < atan2l(v.y,v.x); });
	vector<P> poly; P cur(0,0); ld mx=0;
	for (auto v: e){ poly.push_back(cur); cur = cur + P(v.x, v.y); }
	for (auto p: poly) mx = max({mx, fabsl(p.x), fabsl(p.y)});
	vector<HalfPlane> h; mt19937 mt(3);
	FOR(int, r, 0, 200000/(int)poly.size()) FOR(int, i, 0, (int)poly.size() - 1) h.push_back({poly[i], poly[(i+1)%poly.size()]-poly[i]});
	shuffle(h.begin(), h.end(), mt);
	ld A=0; FOR(int, i, 0, (int)poly.size() - 1) A += poly[i].cross(poly[(i+1)%poly.size()]); A/=2;
	auto res = halfPlaneInter(h, 1e6); 
	ld R=0; FOR(int, i, 0, (int)res.size() - 1) R += res[i].cross(res[(i+1)%res.size()]); R/=2;
	printf("big n=%zu verts=%zu got=%zu maxc=%Lg A=%.3Lf R=%.3Lf\n", h.size(), poly.size(), res.size(), mx, A, R); if (res.size() != poly.size() || fabsl(A - R) > 1e-6L * A) fails++;
	}
	return fails != 0;
}
