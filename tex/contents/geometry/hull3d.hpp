#pragma once
#include "point.hpp"

template<class T> struct Point3{
	typedef Point3 P;
	T x, y, z;
	explicit Point3(T x = 0, T y = 0, T z = 0): x(x), y(y), z(z){}
	bool operator==(P p) const { return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(P p) const { return P(x + p.x, y + p.y, z + p.z); }
	P operator-(P p) const { return P(x - p.x, y - p.y, z - p.z); }
	P operator*(T d) const { return P(x * d, y * d, z * d); }
	P operator/(T d) const { return P(x / d, y / d, z / d); }
	T dot(P p) const { return x * p.x + y * p.y + z * p.z; }
	P cross(P p) const { return P(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x); }
	ld dist() const { return sqrtl((ld)x * x + (ld)y * y + (ld)z * z); }
};
// faces {a, b, c} of the hull, ccw seen from outside; {} if all points are coplanar
template<class P> vector<array<int, 3>> hull3d(const vector<P>& p){
	int n = p.size();
	auto vol = [&](int a, int b, int c, int d){ return sgn((p[b] - p[a]).cross(p[c] - p[a]).dot(p[d] - p[a])); };
	vector<int> o{0};
	EFOR(int, i, 1, n){
		int k = o.size();
		if (k == 1 ? !(p[i] == p[0]) : k == 2 ? !((p[o[1]] - p[0]).cross(p[i] - p[0]) == P()) : k == 3 && vol(0, o[1], o[2], i))
			o.push_back(i);
	}
	if (o.size() < 4) return {};
	vector<array<int, 3>> F;
	EFOR(int, i, 0, 4){ // initial tetrahedron, faces oriented away from the 4th point
		array<int, 3> f; int k = 0;
		EFOR(int, j, 0, 4) if (j != i) f[k++] = o[j];
		if (vol(f[0], f[1], f[2], o[i]) > 0) swap(f[1], f[2]);
		F.push_back(f);
	}
	vector<vector<int>> E(n, vector<int>(n, -1)); // E[a][b] = i: edge a->b on a face visible from i
	EFOR(int, i, 0, n){
		vector<array<int, 3>> keep, vis;
		for (auto& f: F) (vol(f[0], f[1], f[2], i) > 0 ? vis : keep).push_back(f);
		for (auto& f: vis) EFOR(int, j, 0, 3) E[f[j]][f[(j + 1) % 3]] = i;
		for (auto& f: vis) EFOR(int, j, 0, 3){
			int a = f[j], b = f[(j + 1) % 3];
			if (E[b][a] != i) keep.push_back({a, b, i}); // horizon edge
		}
		F = keep;
	}
	return F;
}
// 6 * volume of a closed polyhedron with outward ccw faces (e.g. hull3d), exact for ll
template<class P> auto polyVolume6(const vector<P>& p, const vector<array<int, 3>>& F){
	decltype(p[0].x) v = 0;
	for (auto& f: F) v += (p[f[0]] - p[0]).cross(p[f[1]] - p[0]).dot(p[f[2]] - p[0]);
	return v;
}
template<class P> ld surfaceArea(const vector<P>& p, const vector<array<int, 3>>& F){
	ld s = 0;
	for (auto& f: F) s += (p[f[1]] - p[f[0]]).cross(p[f[2]] - p[f[0]]).dist() / 2;
	return s;
}
