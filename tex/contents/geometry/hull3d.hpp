#pragma once
#include "point.hpp"

// 3D hull, O(n^2) time and n^2 ints. Faces = triples of
// 0-based indices into p, counter-clockwise from outside.
// Coplanar points are fine; exact with Point3<ll>.
template<class T>
struct Point3{
	typedef Point3 P;
	T x = 0, y = 0, z = 0;
	bool operator==(P p) const{
		return tie(x, y, z) == tie(p.x, p.y, p.z);
	}
	P operator+(P p) const{
		return {x + p.x, y + p.y, z + p.z};
	}
	P operator-(P p) const{
		return {x - p.x, y - p.y, z - p.z};
	}
	P operator*(T d) const{ return {x * d, y * d, z * d}; }
	P operator/(T d) const{ return {x / d, y / d, z / d}; }
	T dot(P p) const{ return x * p.x + y * p.y + z * p.z; }
	P cross(P p) const{
		return {y * p.z - z * p.y, z * p.x - x * p.z,
			x * p.y - y * p.x};
	}
	T dist2() const{ return x * x + y * y + z * z; }
	double dist() const{ return sqrt((double)dist2()); }
};
// Faces of the hull of p as index triples, seen
// counter-clockwise from outside. {} if all coplanar.
template<class T>
vector<array<int, 3>> hull3d(const vector<Point3<T>> &p){
	int n = sz(p); vector<int> o(n); iota(all(o), 0);
	auto ok = [&](int s, int i){
		auto v = p[i] - p[o[0]];
		if(s == 1) return !(v == Point3<T>{});
		auto u = p[o[1]] - p[o[0]];
		if(s == 2){
			auto c = u.cross(v);
			return sgn(c.x) || sgn(c.y) || sgn(c.z);
		}
		return sgn(u.cross(p[o[2]] - p[o[0]]).dot(v)) != 0;
	};
	FOR(s, 1, 3){
		int k = s;
		while(k < n && !ok(s, o[k])) k++;
		if(k >= n) return {};
		swap(o[s], o[k]);
	}
	vector<vector<int>> flag(n, vector<int>(n));
	vector<array<int, 3>> f{
		{o[0], o[1], o[2]}, {o[2], o[1], o[0]}}, nf(2);
	FOR(k, 3, n - 1){
		int i = o[k];
		nf.clear();
		for(auto [a, b, c]: f){
			int d = sgn((p[b] - p[a]).cross(p[c] - p[a])
				.dot(p[i] - p[a]));
			if(d <= 0) nf.push_back({a, b, c});
			flag[a][b] = flag[b][c] = flag[c][a] = d;
		}
		for(auto fc: f)
			FOR(e, 0, 2){
				int u = fc[e], v = fc[(e+1)%3];
				if(flag[u][v] > 0 && flag[v][u] <= 0)
					nf.push_back({u, v, i});
			}
		swap(f, nf);
	}
	return f;
}
// 6 * volume of the hull with faces f.
template<class T> T hullVolume6(const vector<Point3<T>> &p,
		const vector<array<int, 3>> &f){
	T v = 0;
	if(f.empty()) return v;
	auto o = p[f[0][0]];
	for(auto [a, b, c]: f)
		v += (p[a] - o).cross(p[b] - o).dot(p[c] - o);
	return v;
}
