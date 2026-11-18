#include "../../tex/contents/ds/lazyseg.hpp"
#include <cassert>

int main(){
	LazySeg s1(vector<LazySeg::Node>(1, {5, 1}));
	assert(s1.get(0, 0).sum == 5);
	s1.update(0, 0, {2, 3});
	assert(s1.get(0, 0).sum == 13);
	s1.update(0, 0, LazySeg::Tag());
	assert(s1.get(0, 0).sum == 13);
	LazySeg::Tag t;
	assert(t.b == 1 && t.c == 0);
	vector<LazySeg::Node> a(6);
	FOR(i, 1, 5)
		a[i] = {i, 1};
	LazySeg s2(a);
	assert(s2.n == 6 && s2.h == 3 && s2.m == 8);
	assert(sz(s2.node) == 16 && sz(s2.tag) == 16);
	assert(s2.get(0, 0).len == 0);
	assert(s2.get(0, 0).sum == 0);
	assert(s2.get(0, 5).sum == 15);
	assert(s2.get(1, 5).len == 5);
	auto neutral = s2.combine(LazySeg::Node(), a[1]);
	assert(neutral.sum == 1 && neutral.len == 1);
	neutral = s2.combine(a[1], LazySeg::Node());
	assert(neutral.sum == 1 && neutral.len == 1);
	s2.apply(1, LazySeg::Tag());
	s2.push(1); s2.pull(1);
	assert(s2.node[1].sum == 15 && s2.tag[1].b == 1);
	vector<ll> brute = {0, 1, 2, 3, 4, 5};
	s2.update(1, 5, {2, 0});
	FOR(i, 1, 5)
		brute[i] *= 2;
	s2.update(2, 3, {1, 1});
	FOR(i, 2, 3)
		brute[i]++;
	s2.update(1, 4, {3, 0});
	FOR(i, 1, 4)
		brute[i] *= 3;
	ll sum = 0;
	FOR(i, 1, 5){
		assert(s2.get(i, i).sum == brute[i]);
		assert(s2.get(i, i).len == 1);
		sum += brute[i];
	}
	assert(s2.get(1, 5).sum == sum);
	assert(s2.get(0, 0).len == 0);
	s1 = LazySeg(a);
	assert(s1.get(1, 5).sum == 15);
	s1.update(1, 5, {0, 9});
	assert(s1.get(1, 5).sum == 45);
	assert(s2.get(1, 5).sum == sum);
	LazySeg s3(vector<LazySeg::Node>(7, {2, 1}));
	s3.update(2, 6, {3, 4});
	FOR(i, 0, 6)
		assert(s3.get(i, i).sum == (i < 2 ? 2 : 10));
	assert(s3.get(0, 6).sum == 54);
	assert(s3.get(0, 6).len == 7);
	LazySeg e = LazySeg(vector<LazySeg::Node>());
	assert(e.n == 0 && e.h == 0 && e.m == 1);
	assert(e.node[1].sum == 0 && e.node[1].len == 0);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	a.assign(n + 1, LazySeg::Node());
	FOR(i, 1, n){
		MintN x; cin >> x;
		a[i] = {x, 1};
	}
	LazySeg seg(a);
	FOR(i, 1, q){
		int type, l, r; cin >> type >> l >> r;
		if(type == 0){
			MintN b, c; cin >> b >> c;
			seg.update(l + 1, r, {b, c});
		}else
			cout << seg.get(l + 1, r).sum << '\n';
	}
}
