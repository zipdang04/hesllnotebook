#pragma once
#include "../contest/template.hpp"

// 3D hull, O(n^2) time and n^2 ints. Exact for |x|, |y|,
// |z| <= 1e9: cross fits ll, dot is __int128.
struct Point3{
	ll x = 0, y = 0, z = 0;
	bool operator==(Point3 p) const{
		return x == p.x && y == p.y && z == p.z;
	}
	Point3 operator-(Point3 p) const{
		return {x - p.x, y - p.y, z - p.z};
	}
	Point3 cross(Point3 p) const{
		return {y * p.z - z * p.y, z * p.x - x * p.z,
			x * p.y - y * p.x};
	}
	__int128 dot(Point3 p) const{
		return (__int128)x * p.x + (__int128)y * p.y
			+ (__int128)z * p.z;
	}
};
// Faces of the hull of p as index triples, seen
// counter-clockwise from outside. {} if all coplanar.
vector<array<int, 3>> hull3d(const vector<Point3> &p){
	int n = sz(p); vector<int> o(n); iota(all(o), 0);
	auto ok = [&](int s, int i){
		auto v = p[i] - p[o[0]];
		if(s == 1) return !(v == Point3{});
		auto u = p[o[1]] - p[o[0]];
		if(s == 2) return !(u.cross(v) == Point3{});
		return u.cross(p[o[2]] - p[o[0]]).dot(v) != 0;
	};
	FOR(s, 1, 3){
		int k = s; while(k < n && !ok(s, o[k])) k++;
		if(k >= n) return {};
		swap(o[s], o[k]);
	}
	vector<vector<int>> flag(n, vector<int>(n));
	vector<array<int, 3>> f{
		{o[0], o[1], o[2]}, {o[2], o[1], o[0]}}, nf;
	FOR(k, 3, n - 1){
		int i = o[k];
		nf.clear();
		for(auto [a, b, c]: f){
			auto t = (p[b] - p[a]).cross(p[c] - p[a])
				.dot(p[i] - p[a]);
			int d = (t > 0) - (t < 0);
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
__int128 hullVolume6(const vector<Point3> &p,
		const vector<array<int, 3>> &f){
	__int128 v = 0;
	if(f.empty()) return v;
	auto o = p[f[0][0]];
	for(auto [a, b, c]: f)
		v += (p[a] - o).cross(p[b] - o).dot(p[c] - o);
	return v;
}
