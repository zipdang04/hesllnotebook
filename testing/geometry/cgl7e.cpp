#include "../../tex/contents/geometry/circle.hpp"
#include <cassert>

// AOJ CGL_7_E: intersection of two circles
bool lessXY(PD a, PD b){
	if(abs(a.x - b.x) > 1e-9)
		return a.x < b.x;
	return a.y < b.y;
}
void selfCheck(){
	auto near = [](PD a, PD b){
		return (a - b).dist() <= 1e-9;
	};
	assert(circleInter({0, 0}, 1, {3, 0}, 1).empty());
	assert(circleInter({0, 0}, 3, {1, 0}, 1).empty());
	assert(circleInter({0, 0}, 1, {0, 0}, 1).empty());
	auto v = circleInter({0, 0}, 1, {2, 0}, 1);
	assert(sz(v) == 1 && near(v[0], {1, 0}));
	v = circleInter({0, 0}, 2, {1, 0}, 1);
	assert(sz(v) == 1 && near(v[0], {2, 0}));
	v = circleInter({0, 0}, 2, {2, 0}, 2);
	assert(sz(v) == 2 && near(v[0], {1, sqrt(3.0)})
		&& near(v[1], {1, -sqrt(3.0)}));
	v = circleLine({2, 1}, 1, {0, 1}, {4, 1});
	assert(sz(v) == 2 && near(v[0], {1, 1})
		&& near(v[1], {3, 1}));
	v = circleLine({2, 1}, 1, {4, 1}, {0, 1});
	assert(sz(v) == 2 && near(v[0], {3, 1})
		&& near(v[1], {1, 1}));
	v = circleLine({2, 1}, 1, {3, 0}, {3, 3});
	assert(sz(v) == 1 && near(v[0], {3, 1}));
	assert(circleLine({2, 1}, 1, {5, 0}, {5, 3}).empty());
	vector<PD> centres = {{6, 0}, {3, 0}, {2, 0}, {1, 0},
		{0.5, 0}, {0, 0}};
	vector<int> counts = {4, 3, 2, 1, 0, 0};
	FOR(i, 0, sz(centres) - 1){
		double r2 = i == 5 ? 2 : 1;
		auto outer = circleTangents({0, 0}, 2, centres[i], r2);
		auto inner = circleTangents({0, 0}, 2, centres[i], -r2);
		assert(sz(outer) + sz(inner) == counts[i]);
	}
	auto t = circleTangents({0, 0}, 2, {0, 4}, 0);
	assert(sz(t) == 2);
	for(auto [a, b]: t){
		assert((b == PD{0, 4}));
		assert(abs(abs(a.x) - sqrt(3.0)) <= 1e-9
			&& abs(a.y - 1) <= 1e-9);
	}
	assert(abs(t[0].fi.x + t[1].fi.x) <= 1e-9);
	t = circleTangents({0, 0}, 2, {0, 2}, 0);
	assert(sz(t) == 1 && near(t[0].fi, {0, 2}));
	assert((t[0].se == PD{0, 2}));
	assert(near(circumcenter({0, 0}, {4, 0}, {0, 3}),
		{2, 1.5}));
	auto [o, r] = enclosingCircle({{1, 1}});
	assert(near(o, {1, 1}) && abs(r) <= 1e-9);
	tie(o, r) = enclosingCircle({{0, 0}, {1, 0}, {1, 0},
		{4, 0}});
	assert(near(o, {2, 0}) && abs(r - 2) <= 1e-9);
	tie(o, r) = enclosingCircle({{0, 0}, {4, 0}, {0, 3},
		{1, 1}});
	assert(near(o, {2, 1.5}) && abs(r - 2.5) <= 1e-9);
}
int main(){
	selfCheck();
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << fixed << setprecision(10);
	PD c1, c2; double r1, r2;
	cin >> c1.x >> c1.y >> r1 >> c2.x >> c2.y >> r2;
	auto v = circleInter(c1, r1, c2, r2);
	if(sz(v) == 1)
		v.push_back(v[0]);
	sort(all(v), lessXY);
	cout << v[0].x << ' ' << v[0].y << ' '
		<< v[1].x << ' ' << v[1].y << '\n';
}
