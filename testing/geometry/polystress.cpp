#include "../../tex/contents/geometry/polygon.hpp"
typedef Point<ll> P; typedef Point<ld> Q;
mt19937_64 mt(7);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
bool simple(const vector<P>& v){
	int n = v.size();
	EFOR(int, i, 0, n) EFOR(int, j, i + 1, n){
		P a = v[i], b = v[(i + 1) % n], c = v[j], d = v[(j + 1) % n];
		auto s = segInter(a, b, c, d);
		bool adj = j == i + 1 || (i == 0 && j == n - 1);
		if (!adj && !s.empty()) return false;
		if (adj && s.size() != 1) return false;
	}
	return true;
}
vector<P> gen(int n, ll C){ // random simple polygon (star-shaped around lowest point), either orientation
	while (1){
		vector<P> v(n);
		for (auto& p: v) p = P(rnd(-C, C), rnd(-C, C));
		swap(v[0], *min_element(v.begin(), v.end()));
		P o = v[0];
		sort(v.begin() + 1, v.end(), [&](P a, P b){ ll c = o.cross(a, b); return c ? c > 0 : (a - o).dist2() < (b - o).dist2(); });
		if (rnd(0, 1)) reverse(v.begin(), v.end());
		if (polyArea2(v) != 0 && simple(v)) return v;
	}
}
// brute classification: exact ray cast to +x with rationals
int bruteIn(const vector<P>& v, P a){
	int n = v.size(), c = 0;
	EFOR(int, i, 0, n){
		P p = v[i], q = v[(i + 1) % n];
		if (p.cross(q, a) == 0 && min(p.x, q.x) <= a.x && a.x <= max(p.x, q.x) && min(p.y, q.y) <= a.y && a.y <= max(p.y, q.y)) return 0;
		if ((p.y > a.y) != (q.y > a.y)){
			// x of crossing = p.x + (a.y-p.y)(q.x-p.x)/(q.y-p.y) > a.x ?
			__int128 num = (__int128)(a.y - p.y) * (q.x - p.x), den = q.y - p.y;
			__int128 lhs = (__int128)(p.x - a.x) * den + num; // sign(lhs/den) > 0
			if ((lhs > 0) == (den > 0) && lhs != 0) c ^= 1;
		}
	}
	return c ? 1 : -1;
}
__int128 area128(const vector<P>& v){ __int128 a = 0; int n = v.size(); EFOR(int, i, 0, n) a += (__int128)v[i].x * v[(i+1)%n].y - (__int128)v[i].y * v[(i+1)%n].x; return a; }
ld cutArea(vector<Q> v, Q s, Q e){ auto r = polygonCut(v, s, e); return r.size() < 3 ? 0 : polyArea2(r) / 2; }
int cuts=0, maxn=0;
int main(){
	FOR(int, it, 1, 3000){
		ll C = rnd(1, 7); int n = rnd(3, C < 3 ? 4 : 8);
		auto v = gen(n, C);
		// Pick + inPolygon on lattice points
		ll I = 0, B = 0;
		FOR(ll, x, -C - 1, C + 1) FOR(ll, y, -C - 1, C + 1){
			int r = inPolygon(v, P(x, y)), b = bruteIn(v, P(x, y));
			if (r != b){ printf("FAIL inPolygon it=%d\n", it); return 1; }
			I += r == 1; B += r == 0;
		}
		ll Bg = 0; EFOR(int, i, 0, n) Bg += __gcd(llabs(v[i].x - v[(i+1)%n].x), llabs(v[i].y - v[(i+1)%n].y));
		if (Bg != B || llabs(polyArea2(v)) != 2 * I + B - 2){ printf("FAIL pick it=%d\n", it); return 1; }
		// cut on non-convex: compare with fan decomposition of triangles (signed)
		vector<Q> w; for (P p: v) w.push_back(Q(p.x, p.y));
		Q s(rnd(-C, C), rnd(-C, C)), e(rnd(-C, C), rnd(-C, C)); if (s == e) continue;
		cuts++; maxn=max(maxn,n); ld got = cutArea(w, s, e), exp = 0;
		EFOR(int, i, 0, n){
			vector<Q> t = {w[0] * 0 + Q(-C - 3, -C - 3), w[i], w[(i + 1) % n]};
			ld sg = polyArea2(t) > 0 ? 1 : -1; if (sg < 0) swap(t[1], t[2]);
			exp += sg * cutArea(t, s, e);
		}
		if (abs(got - exp) > 1e-9 || abs(got + cutArea(w, e, s) - polyArea2(w) / 2) > 1e-9){ printf("FAIL cut it=%d %Lf %Lf\n", it, got, exp); return 1; }
	}
	// large coords: inPolygon near 1e9 (vertices, edge midpoints, nearby and random points)
	FOR(int, it, 1, 3000){
		ll C = 1000000000; int n = rnd(3, 8); auto v = gen(n, C);
		vector<P> qs;
		EFOR(int, i, 0, n){ P p = v[i], q = v[(i + 1) % n]; qs.push_back(p); qs.push_back(P((p.x + q.x) / 2, (p.y + q.y) / 2)); qs.push_back(p + P(rnd(-1, 1), rnd(-1, 1))); }
		EFOR(int, k, 0, 10) qs.push_back(P(rnd(-C, C), rnd(-C, C)));
		for (P a: qs) if (llabs(a.x) <= C && llabs(a.y) <= C && inPolygon(v, a) != bruteIn(v, a)){ printf("FAIL big inPolygon\n"); return 1; }
		if ((__int128)polyArea2(v) != area128(v)){ printf("FAIL big area simple\n"); return 1; }
	}
	printf("OK cuts=%d maxn=%d\n",cuts,maxn);
}
