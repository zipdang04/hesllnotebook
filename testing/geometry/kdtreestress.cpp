#include "../../tex/contents/geometry/closestpair.hpp"

typedef Point<ll> P;
mt19937_64 rng(12345);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(rng); }

int main(){
	const ll LIM = 1e9;
	int cases = 0;
	FOR(int, it, 0, 19999){
		int mode = it % 5;
		ll C = mode == 0 ? 3 : mode == 1 ? 20 : mode == 2 ? 1000 : LIM;
		int n = rnd(1, mode == 0 ? 8 : 60);
		vector<P> v;
		FOR(int, i, 0, n - 1){
			if (mode == 4) v.push_back(P(rnd(0, 1) ? LIM : -LIM, rnd(-LIM, LIM))); // two extreme vertical lines
			else if (i && rnd(0, 4) == 0) v.push_back(v[rnd(0, i - 1)]);          // duplicate
			else v.push_back(P(rnd(-C, C), rnd(-C, C)));
		}
		KDTree kd(v);
		auto fail = [&](string what, P q, int skip, pair<ll, int> got, ll exp){
			printf("FAIL %s: n=%d q=(%lld,%lld) skip=%d got=(%lld,%d) expected dist2=%lld\npoints:", what.c_str(), n, q.x, q.y, skip, got.fi, got.se, exp);
			for (P p: v) printf(" (%lld,%lld)", p.x, p.y);
			puts(""); exit(1);
		};
		FOR(int, qi, 0, (2 * n + 5) - 1){
			P q; int skip = -1;
			if (qi < n) q = v[qi], skip = qi;                         // all-nearest-neighbour style
			else if (qi < 2 * n) q = v[qi - n];                       // query at a point, no skip -> 0
			else q = mode == 4 ? P(rnd(-LIM, LIM), rnd(0, 1) ? LIM : -LIM) : P(rnd(-C, C), rnd(-C, C));
			ll exp = LLONG_MAX;
			FOR(int, i, 0, n - 1) if (i != skip) exp = min(exp, (v[i] - q).dist2());
			auto got = kd.nearest(q, skip);
			if (got.fi != exp) fail("dist", q, skip, got, exp);
			if (exp == LLONG_MAX ? got.se != -1 : (got.se < 0 || got.se >= n || got.se == skip || (v[got.se] - q).dist2() != exp))
				fail("index", q, skip, got, exp);
		}
		if (n >= 2){ // closestPair vs brute (also checked by LC geo/closest_pair)
			ll exp = LLONG_MAX;
			FOR(int, i, 0, n - 1) FOR(int, j, 0, i - 1) exp = min(exp, (v[i] - v[j]).dist2());
			auto [a, b] = closestPair(v);
			int ia = find(v.begin(), v.end(), a) - v.begin(), ib = find(v.begin(), v.end(), b) - v.begin();
			if (ia == n || ib == n || (a - b).dist2() != exp || (a == b && count(v.begin(), v.end(), a) < 2)){
				printf("FAIL closestPair: got dist2=%lld expected=%lld\n", (a - b).dist2(), exp); exit(1);
			}
		}
		cases++;
	}
	// big: n = 2e5 random points in [-1e9, 1e9]^2, 2e5 skip-self queries checked against closestPair
	int n = 200000;
	vector<P> v(n);
	for (auto& p: v) p = P(rnd(-LIM, LIM), rnd(-LIM, LIM));
	auto t0 = clock();
	KDTree kd(v);
	ll best = LLONG_MAX;
	FOR(int, i, 0, n - 1) best = min(best, kd.nearest(v[i], i).fi);
	double secs = double(clock() - t0) / CLOCKS_PER_SEC;
	auto [a, b] = closestPair(v);
	if (best != (a - b).dist2()){ printf("FAIL big: kd %lld vs closestPair %lld\n", best, (a - b).dist2()); return 1; }
	printf("OK %d cases; big n=%d all-nearest %.3fs, min dist2=%lld\n", cases, n, secs, best);
}
