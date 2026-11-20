#include "../../tex/contents/geometry/hull3d.hpp"
#include <cassert>

int main(){
	typedef Point3 P3;
	assert((P3{} == P3{0, 0, 0}));
	assert(!(P3{1, 2, 3} == P3{1, 2, 4}));
	assert((P3{1, 2, 3} - P3{1, 1, 1} == P3{0, 1, 2}));
	assert((P3{1, 0, 0}.cross(P3{0, 1, 0})
		== P3{0, 0, 1}));
	assert((P3{1, 2, 3}.dot(P3{4, 5, 6}) == 32));
	vector<P3> p{{0, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {0, 0, 1}};
	auto f = hull3d(p);
	assert(sz(f) == 4);
	assert(hullVolume6(p, f) == 1);
	for(auto [a, b, c]: f){ // the 4th vertex is behind each face
		P3 normal = (p[b] - p[a]).cross(p[c] - p[a]);
		assert(normal.dot(p[6 - a - b - c] - p[a]) < 0);
	}
	vector<P3> cube;
	for(ll x: {0, 3})
		for(ll y: {0, 3})
			for(ll z: {0, 3})
				cube.push_back({x, y, z});
	f = hull3d(cube);
	assert(sz(f) == 12);
	assert(hullVolume6(cube, f) == 162);
	FOR(n, 0, 3){
		vector<P3> small(p.begin(), p.begin() + n);
		assert(hull3d(small).empty());
	}
	assert(hull3d(vector<P3>{{0, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {1, 1, 0}, {1, 0, 0}}).empty());
	assert(hull3d(vector<P3>{{0, 0, 0}, {1, 2, 3},
		{2, 4, 6}, {3, 6, 9}}).empty());
	assert(hull3d(vector<P3>(5, {7, 8, 9})).empty());
	p = {{1, 1, 0}, {1, 1, 1}, {1, 0, 0}, {0, 0, 0}};
	for(P3 q: cube)
		p.push_back({q.x * 2 / 3, q.y * 2 / 3, q.z * 2 / 3});
	f = hull3d(p);
	assert(hullVolume6(p, f) == 48);
	map<pii, int> edges;
	for(auto fc: f){
		auto [a, b, c] = fc;
		P3 normal = (p[b] - p[a]).cross(p[c] - p[a]);
		assert(!(normal == P3{}));
		for(P3 q: p)
			assert(normal.dot(q - p[a]) <= 0);
		FOR(e, 0, 2)
			edges[{fc[e], fc[(e+1)%3]}]++;
	}
	for(auto [edge, cnt]: edges){
		auto [a, b] = edge;
		assert(cnt == 1);
		assert(edges.at({b, a}) == 1);
	}
	p.clear();
	for(int i: {6, 2, 7, 0, 5, 1, 4, 3})
		p.push_back(cube[i]);
	f = hull3d(p);
	for(auto fc: f)
		for(int i: fc)
			assert(0 <= i && i < 8);
	assert(hullVolume6(p, f) == 162);
	p.clear();
	const ll C = 1000000000; // the limit: cube corners plus points on its faces
	for(ll x: {-C, C})
		for(ll y: {-C, C})
			for(ll z: {-C, C})
				p.push_back({x, y, z});
	mt19937_64 rng(3);
	FOR(k, 1, 200){
		ll a = (ll)(rng() % (2 * C + 1)) - C, b = (ll)(rng() % (2 * C + 1)) - C;
		ll w = rng() % 2 ? C : -C;
		int ax = rng() % 3;
		p.push_back(ax == 0 ? P3{w, a, b} : ax == 1 ? P3{a, w, b} : P3{a, b, w});
	}
	shuffle(all(p), rng);
	f = hull3d(p);
	for(auto [a, b, c]: f) // the C^3 products need __int128
		for(P3 q: p)
			assert((p[b] - p[a]).cross(p[c] - p[a]).dot(q - p[a]) <= 0);
	assert(hullVolume6(p, f) == (__int128)6 * 8 * C * C * C);
	assert(hullVolume6(p, {}) == 0);
	// decimals x10: 3 collinear, 2 coplanar groups
	p = {{7067, 2933, 2503}, {7067, 5433, 3},
		{-433, 5433, 7503}, {4567, 7933, 3},
		{7067, 433, 5003}, {2067, 7933, 3}};
	f = hull3d(p);
	assert(sz(f) == 8);
	assert(hullVolume6(p, f) == 140625000000LL);
	return 0;
}
