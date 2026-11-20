#include "../../tex/contents/geometry/hull3d.hpp"

using P3 = Point3;
void fail(const vector<P3> &p, const char *why){
	cerr << why << '\n' << sz(p) << '\n';
	for(auto q: p)
		cerr << q.x << ' ' << q.y << ' ' << q.z << '\n';
	exit(1);
}
// Independent determinant, without Point3 arithmetic.
__int128 det(P3 a, P3 b, P3 c, P3 o){
	ll ax = a.x - o.x, ay = a.y - o.y, az = a.z - o.z;
	ll bx = b.x - o.x, by = b.y - o.y, bz = b.z - o.z;
	ll cx = c.x - o.x, cy = c.y - o.y, cz = c.z - o.z;
	return (__int128)ax * (by * cz - bz * cy)
		+ (__int128)ay * (bz * cx - bx * cz)
		+ (__int128)az * (bx * cy - by * cx);
}
__int128 check(const vector<P3> &p){
	int n = sz(p);
	bool flat = true, general = true;
	FOR(a, 0, n - 1)
		FOR(b, a + 1, n - 1)
			FOR(c, b + 1, n - 1)
				FOR(d, c + 1, n - 1){
					if(det(p[b], p[c], p[d], p[a])) flat = false;
					else general = false;
				}
	auto f = hull3d(p);
	if(f.empty() != flat) fail(p, "flatness");
	map<pii, int> edges;
	vector<array<int, 3>> faces;
	for(auto t: f){
		for(int i: t)
			if(i < 0 || i >= n) fail(p, "face index");
		auto [a, b, c] = t;
		auto u = (p[b] - p[a]).cross(p[c] - p[a]);
		if(!u.x && !u.y && !u.z) fail(p, "zero normal");
		for(auto q: p)
			if(det(p[b], p[c], q, p[a]) > 0)
				fail(p, "point in front");
		FOR(e, 0, 2) edges[{t[e], t[(e+1)%3]}]++;
		if(b < a && b < c) t = {b, c, a};
		else if(c < a && c < b) t = {c, a, b};
		faces.push_back(t);
	}
	for(auto [e, cnt]: edges){
		auto it = edges.find({e.se, e.fi});
		if(cnt != 1 || it == edges.end() || it->se != 1)
			fail(p, "edge multiplicity");
	}
	__int128 got = hullVolume6(p, f);
	if(flat ? got != 0 : got <= 0) fail(p, "volume sign");
	if(flat || !general) return got;
	vector<array<int, 3>> brute;
	__int128 want = 0;
	FOR(a, 0, n - 1)
		FOR(b, a + 1, n - 1)
			FOR(c, a + 1, n - 1){
				if(b == c) continue;
				bool ok = true;
				FOR(d, 0, n - 1){
					if(d == a || d == b || d == c) continue;
					if(det(p[b], p[c], p[d], p[a]) >= 0){
						ok = false;
						break;
					}
				}
				if(!ok) continue;
				brute.push_back({a, b, c});
				want += det(p[a], p[b], p[c], p[0]);
			}
	sort(all(faces));
	sort(all(brute));
	if(faces != brute) fail(p, "supporting triples");
	if(got != want) fail(p, "brute volume");
	return got;
}
int main(){
	mt19937_64 rng(712367);
	check({});
	FOR(it, 0, 199999){
		int n = 1 + rng() % 12, side = 1 + rng() % 4;
		vector<P3> p;
		FOR(i, 1, n){
			if(it % 5 == 0)
				p.push_back({(ll)(rng() % 2000000001) - 1000000000,
					(ll)(rng() % 2000000001) - 1000000000,
					(ll)(rng() % 2000000001) - 1000000000});
			else if(it % 5 == 1) // small grid scaled to 1e9: coplanar at the limit
				p.push_back({(ll)(rng() % side) * 333333333,
					(ll)(rng() % side) * 333333333,
					(ll)(rng() % side) * 333333333});
			else
				p.push_back({(ll)(rng() % side),
					(ll)(rng() % side), (ll)(rng() % side)});
		}
		__int128 v = check(p);
		shuffle(all(p), rng);
		if(check(p) != v) fail(p, "shuffle volume");
	}
	vector<P3> p;
	FOR(x, -30LL, 29)
		FOR(y, -25LL, 24)
			p.push_back({x, y, x * x + y * y});
	auto start = chrono::steady_clock::now();
	auto f = hull3d(p);
	auto elapsed = chrono::steady_clock::now() - start;
	auto seconds = chrono::duration<double>(elapsed).count();
	if(seconds >= 1) fail(p, "paraboloid time >= 1s");
	vector<char> used(sz(p));
	for(auto t: f)
		for(int i: t){
			if(i < 0 || i >= sz(p)) fail(p, "speed face index");
			used[i] = 1;
		}
	for(char v: used) if(!v) fail(p, "missing hull vertex");
	if(hullVolume6(p, f) <= 0) fail(p, "speed volume");
	return 0;
}
