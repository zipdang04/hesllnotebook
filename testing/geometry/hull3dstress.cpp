#include "../../tex/contents/geometry/hull3d.hpp"
#include "../../tex/contents/geometry/hull.hpp"
typedef Point3<ll> P;
typedef __int128 L;
// brute: for every supporting plane (all points on one side), 2D hull of its points, fan-sum
bool brute(const vector<P>& p, ll& v6, ld& area){
	int n = p.size(); v6 = 0; area = 0;
	set<array<ll, 4>> seen; bool any = false;
	EFOR(int, i, 0, n) EFOR(int, j, 0, n) EFOR(int, k, 0, n){
		P nv = (p[j] - p[i]).cross(p[k] - p[i]);
		if (nv == P()) continue;
		int pos = 0, neg = 0;
		for (auto& q: p){ L d = (L)nv.dot(q - p[i]); pos |= d > 0; neg |= d < 0; }
		if (pos && neg) continue;
		if (!pos && !neg) return false; // all coplanar
		any = true;
		if (pos) nv = nv * -1;
		ll g = __gcd(__gcd(llabs(nv.x), llabs(nv.y)), llabs(nv.z));
		nv = nv / g;
		array<ll, 4> key{nv.x, nv.y, nv.z, nv.dot(p[i])};
		if (!seen.insert(key).second) continue;
		vector<int> S;
		EFOR(int, t, 0, n) if (nv.dot(p[t] - p[i]) == 0) S.push_back(t);
		// project: drop axis with largest |normal|
		int ax = llabs(nv.x) >= llabs(nv.y) && llabs(nv.x) >= llabs(nv.z) ? 0 : llabs(nv.y) >= llabs(nv.z) ? 1 : 2;
		vector<pair<Point<ll>, int>> q;
		for (int t: S){ ll c[3] = {p[t].x, p[t].y, p[t].z}; q.push_back({Point<ll>(c[(ax + 1) % 3], c[(ax + 2) % 3]), t}); }
		vector<Point<ll>> pts; for (auto& e: q) pts.push_back(e.fi);
		auto h = convexHull(pts);
		vector<int> poly;
		for (auto& hp: h) for (auto& e: q) if (e.fi == hp){ poly.push_back(e.se); break; }
		if (poly.size() < 3) { printf("bad poly\n"); exit(1); }
		// orient outward
		P fn = (p[poly[1]] - p[poly[0]]).cross(p[poly[2]] - p[poly[0]]);
		if (fn.dot(nv) < 0) reverse(poly.begin(), poly.end());
		EFOR(int, t, 1, (int)poly.size() - 1){
			P c = (p[poly[t]] - p[poly[0]]).cross(p[poly[t + 1]] - p[poly[0]]);
			area += sqrtl((ld)c.x * c.x + (ld)c.y * c.y + (ld)c.z * c.z) / 2;
			L a1[3] = {p[poly[0]].x - p[0].x, p[poly[0]].y - p[0].y, p[poly[0]].z - p[0].z};
			L b1[3] = {p[poly[t]].x - p[0].x, p[poly[t]].y - p[0].y, p[poly[t]].z - p[0].z};
			L c1[3] = {p[poly[t+1]].x - p[0].x, p[poly[t+1]].y - p[0].y, p[poly[t+1]].z - p[0].z};
			L cr[3] = {a1[1]*b1[2]-a1[2]*b1[1], a1[2]*b1[0]-a1[0]*b1[2], a1[0]*b1[1]-a1[1]*b1[0]};
			v6 += (ll)(cr[0]*c1[0] + cr[1]*c1[1] + cr[2]*c1[2]);
		}
	}
	return any;
}
int main(){
	mt19937_64 mt(1);
	auto rnd = [&](ll a, ll b){ return (ll)(mt() % (b - a + 1)) + a; };
	const ll C = 500000;
	int ITER = 30000;
	EFOR(int, it, 0, ITER){
		int n = rnd(1, it % 5 ? 14 : 6);
		int shape = it % 6;
		ll K = vector<ll>{1, 2, 3, 10, 1000}[it % 5];
		vector<P> p;
		P base;
		if (it % 4 == 1) base = P(rnd(0,1) ? C - K : -C + K, rnd(0,1) ? C - K : -C + K, rnd(0,1) ? C - K : -C + K);
		EFOR(int, i, 0, n){
			P q(rnd(-K, K), rnd(-K, K), rnd(-K, K));
			if (shape == 1) q.z = 0;                       // all coplanar
			if (shape == 2) q.z = q.x + q.y;               // coplanar, tilted
			if (shape == 3) q = P(q.x, 2 * q.x, -q.x);      // collinear
			if (shape == 4 && i % 2) q.z = rnd(0, 1) ? -K : K; // many on two planes
			p.push_back(base + q);
		}
		if (it % 9 == 4) EFOR(int, i, 0, n) p[i] = P(rnd(0,1) ? C : -C, rnd(0,1) ? C : -C, rnd(0,1) ? C : -C);
		if (shape == 5 && n > 1) p[rnd(0, n - 1)] = p[0];    // duplicate
		ll bv; ld ba; bool full = brute(p, bv, ba);
		auto F = hull3d(p);
		bool ok = true;
		if (!full) ok = F.empty();
		else {
			// checks: supporting, non-degenerate, closed (each directed edge once, reverse present)
			map<pair<int,int>, int> E;
			for (auto& f: F){
				P nv = (p[f[1]] - p[f[0]]).cross(p[f[2]] - p[f[0]]);
				if (nv == P()) ok = false;
				for (auto& q: p) if ((L)nv.dot(q - p[f[0]]) > 0) ok = false;
				EFOR(int, j, 0, 3) E[{f[j], f[(j + 1) % 3]}]++;
			}
			for (auto& [e, c]: E) if (c != 1 || !E.count({e.se, e.fi})) ok = false;
			if (polyVolume6(p, F) != bv) ok = false;
			if (abs(surfaceArea(p, F) - ba) > 1e-9 * max((ld)1, ba)) ok = false;
		}
		if (!ok){
			printf("FAIL it=%d n=%d full=%d faces=%zu bv=%lld got=%lld\n", it, n, full, F.size(), bv, full ? polyVolume6(p, F) : 0LL);
			for (auto& q: p) printf("%lld %lld %lld\n", q.x, q.y, q.z);
			return 1;
		}
	}
	// timing: n = 2000 random points on a sphere-ish shell
	vector<P> big;
	EFOR(int, i, 0, 2000){ ld a = rnd(0, 1e9) / 1e9 * 6.283, b = rnd(0, 1e9) / 1e9 * 3.1415;
		big.push_back(P(llroundl(C * cosl(a) * sinl(b)), llroundl(C * sinl(a) * sinl(b)), llroundl(C * cosl(b)))); }
	auto t0 = clock(); auto F = hull3d(big);
	printf("n=2000 shell: %zu faces, %.3fs\n", F.size(), (double)(clock() - t0) / CLOCKS_PER_SEC);
	puts("OK");
}
