#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
mt19937 mt(1);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
ld brute(vector<P> &ps){ // O(n^4): best circle through 2 (diameter) or 3 points enclosing all
	int n = ps.size(); ld best = 1e30;
	auto ok = [&](P o, ld r){ for (P p: ps) if ((p - o).dist() > r * (1 + 1e-12) + 1e-12) return false; return true; };
	if (n == 1) return 0;
	EFOR(int, i, 0, n) EFOR(int, j, i, n){
		P o = (ps[i] + ps[j]) / 2; ld r = (o - ps[i]).dist(); if (r < best && ok(o, r)) best = r;
		EFOR(int, k, j + 1, n){
			P a = ps[i], b = ps[j], c = ps[k]; ld D = 2 * (b - a).cross(c - a); if (D == 0) continue;
			// solve |o-a|=|o-b|=|o-c| by Cramer
			ld b1 = b.dist2() - a.dist2(), c1 = c.dist2() - a.dist2();
			P q((b1 * (c.y - a.y) - c1 * (b.y - a.y)) / D, ((b.x - a.x) * c1 - (c.x - a.x) * b1) / D);
			ld r2 = (q - a).dist(); if (r2 < best && ok(q, r2)) best = r2;
		}
	}
	return best;
}
int main(int argc, char **argv){
	ll C[] = {3, 10, 1000, 1000000, 1000000000};
	FOR(int, it, 1, 20000){
		int n = rnd(1, 12), sh = rnd(0, 3); ll R = C[rnd(0, 4)];
		vector<P> ps;
		EFOR(int, i, 0, n){
			if (sh == 0) ps.push_back(P(rnd(-R, R), rnd(-R, R)));
			else if (sh == 1) { ll t = rnd(-R, R); ps.push_back(P(t, 2 * t / 3)); } // collinear-ish
			else if (sh == 2) ps.push_back(i ? ps[0] : P(rnd(-R, R), rnd(-R, R)));
			else { ld a = rnd(0, 359) * acosl(-1) / 180; ps.push_back(P(roundl(R * cosl(a)), roundl(R * sinl(a)))); }
		}
		auto [o, r] = mec(ps); ld b = brute(ps);
		bool bad = abs(r - b) > 1e-6 * max((ld)1, b);
		for (P p: ps) bad |= (p - o).dist() > r * (1 + 1e-7) + 1e-9;
		if (bad){ printf("FAIL it=%d n=%d r=%.12Lf brute=%.12Lf\n", it, n, r, b); for (P p: ps) printf("%.0Lf %.0Lf\n", p.x, p.y); return 1; }
	}
	puts("OK");
	// timing
	vector<P> ps; EFOR(int, i, 0, 1000000) ps.push_back(P(rnd(-1e9, 1e9), rnd(-1e9, 1e9)));
	auto t0 = clock(); auto [o, r] = mec(ps); printf("1e6 pts: %.3fs r=%.3Lf\n", (double)(clock() - t0) / CLOCKS_PER_SEC, r);
}
