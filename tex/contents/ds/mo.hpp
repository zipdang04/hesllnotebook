#pragma once
#include "../contest/template.hpp"
template<class A, class D, class C>
auto mo(const vector<pair<int,int>> &Q,
		A add, D del, C calc){
	int q = Q.size(), L = 1, R = 0, n = 1;
	for (auto [l, r]: Q) n = max(n, r);
	int B = max(1, int(n / sqrt(q)));
	vector<int> s(q); vector<decltype(calc())> res(q);
	iota(s.begin(), s.end(), 0);
	auto key = [&](int i){
		int b = Q[i].fi / B;
		return pair(b, b & 1 ? -Q[i].se : Q[i].se);
	};
	sort(s.begin(), s.end(), [&](int a, int b){
		return key(a) < key(b);
	});
	for (int i: s){
		auto [l, r] = Q[i];
		while (L > l) add(--L, 0);
		while (R < r) add(++R, 1);
		while (L < l) del(L++, 0);
		while (R > r) del(R--, 1);
		res[i] = calc();
	}
	return res;
}
