#include "../../tex/contents/geometry/hull.hpp"
typedef Point<ll> P;
mt19937_64 mt(7);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
int bruteIn(const vector<P>& h, P p){
	int n = h.size();
	if (n < 3) return onSegment(h[0], h.back(), p) ? 0 : -1;
	EFOR(int, i, 0, n) if (orient(h[i], h[(i+1)%n], p) < 0) return -1;
	EFOR(int, i, 0, n) if (onSegment(h[i], h[(i+1)%n], p)) return 0;
	return 1;
}
int main(){
	EFOR(int, it, 0, 300000){
		int K = it % 4 == 0 ? 2 : (it % 4 == 1 ? 6 : 30); ll off = it % 3 == 0 ? 1000000000 - K : 0; ll sg = it%2?1:-1;
		int n = rnd(1, it % 5 == 0 ? 4 : 25);
		vector<P> v(n); bool line = it % 7 == 0; ll dx = rnd(-2,2), dy = rnd(-2,2);
		for (auto& p: v){ if (line){ ll t = rnd(-K, K); p = P(off*sg + dx*t/ (K>2?1:1), -off*sg + dy*t);} else if (it % 11 == 3){ ll c[5]={-1000000000,-999999999,0,999999999,1000000000}; p = P(c[rnd(0,4)], c[rnd(0,4)]); } else p = P(sg*off + rnd(-K, K), -sg*off + rnd(-K, K)); }
		auto h = convexHull(v);
		// brute hull check: every input point bruteIn >= 0, every hull vertex is a strict vertex
		for (P p: v) if (bruteIn(h, p) < 0){ puts("hull misses"); return 1; }
		int m = h.size();
		if (m >= 3) EFOR(int, i, 0, m) if (orient(h[i], h[(i+1)%m], h[(i+2)%m]) <= 0){ puts("not strict ccw"); return 1; }
		set<P> sv(v.begin(), v.end()); if (m == 2 && h[0] == h[1]){ puts("dup"); return 1;} if (sv.size()==1 && m!=1){puts("single");return 1;}
		if (sv.size() >= 2 && m < 2){ puts("too small"); return 1; }
		auto d = hullDiameter(h); ll best = 0;
		for (P a: v) for (P b: v) best = max(best, (a-b).dist2());
		if ((d[0]-d[1]).dist2() != best){ printf("diam %d\n", it); return 1; }
		EFOR(int, q, 0, 30){
			P p(sg*off + rnd(-K-2, K+2), -sg*off + rnd(-K-2, K+2));
			if (q < 5) p = v[rnd(0, n-1)]; if (it % 11 == 3 && q >= 5){ ll c[7]={-1000000000,-999999999,-1,0,1,999999999,1000000000}; p = P(c[rnd(0,6)], c[rnd(0,6)]); }
			if (inHull(h, p) != bruteIn(h, p)){ printf("in %d %d %d\n", it, inHull(h,p), bruteIn(h,p)); return 1; }
		}
	}
	puts("OK");
}
