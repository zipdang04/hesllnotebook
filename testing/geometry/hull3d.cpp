#include "../../tex/contents/geometry/hull3d.hpp"
#include <cassert>

int main(){
	typedef Point3<ll> P3;
	assert((P3{} == P3{0, 0, 0}));
	assert(!(P3{1, 2, 3} == P3{1, 2, 4}));
	assert((P3{1, 2, 3} + P3{1, 1, 1} == P3{2, 3, 4}));
	assert((P3{1, 2, 3} - P3{1, 1, 1} == P3{0, 1, 2}));
	assert(((P3{1, 2, 3} * 4) / 2 == P3{2, 4, 6}));
	assert((P3{1, 0, 0}.cross(P3{0, 1, 0})
		== P3{0, 0, 1}));
	assert((P3{1, 2, 3}.dot(P3{4, 5, 6}) == 32));
	assert((P3{2, 3, 6}.dist2() == 49));
	assert((P3{2, 3, 6}.dist() == 7));
	vector<P3> p{{0, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {0, 0, 1}};
	auto f = hull3d(p);
	assert(sz(f) == 4);
	assert(hullVolume6(p, f) == 1);
	P3 s{};
	for(P3 q: p)
		s = s + q;
	for(auto [a, b, c]: f){
		P3 normal = (p[b] - p[a]).cross(p[c] - p[a]);
		assert(normal.dot(p[a] * 4 - s) > 0);
	}
	vector<P3> cube;
	vector<Point3<double>> realCube;
	for(ll x: {0, 3})
		for(ll y: {0, 3})
			for(ll z: {0, 3}){
				cube.push_back({x, y, z});
				realCube.push_back({(double)x, (double)y,
					(double)z});
			}
	f = hull3d(cube);
	assert(sz(f) == 12);
	assert(hullVolume6(cube, f) == 162);
	auto realFaces = hull3d(realCube);
	assert(sz(realFaces) == 12);
	assert(abs(hullVolume6(realCube, realFaces) - 162)
		< 1e-9);
	for(auto &q: realCube)
		q = q * 0.37 + Point3<double>{0.123, 0.123, 0.123};
	realCube.push_back({0.4, 0.5, 0.6});
	realCube.push_back({0.7, 0.8, 0.9});
	realFaces = hull3d(realCube);
	assert(sz(realFaces) == 12);
	assert(abs(hullVolume6(realCube, realFaces) - 8.205786)
		< 1e-6);
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
		p.push_back(q * 2 / 3);
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
	for(ll x: {-500000, 500000})
		for(ll y: {-500000, 500000})
			for(ll z: {-500000, 500000})
				p.push_back({x, y, z});
	f = hull3d(p);
	assert(hullVolume6(p, f) == 6000000000000000000LL);
	assert(hullVolume6(p, {}) == 0);
	return 0;
}
