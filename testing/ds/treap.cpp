#include "../../tex/contents/ds/treap.hpp"
#include <cassert>

void check(Treap &tr, const vector<ll> &brute){
	int n = sz(brute);
	assert(tr.t[tr.root].size == n);
	ll sum = 0;
	FOR(i, 1, n){
		assert(tr.get(i, i).sum == brute[i - 1]);
		assert(tr.get(i, i).size == 1);
		sum += brute[i - 1];
	}
	if(n) assert(tr.get(1, n).sum == sum);
	if(n) assert(tr.get(1, n).size == n);
	assert(tr.t[0].size == 0 && tr.t[0].sum == 0);
	assert(!tr.t[0].rev && tr.t[0].val == 0);
	assert(tr.t[0].l == 0 && tr.t[0].r == 0);
	assert(tr.t[0].tag.b == 1 && tr.t[0].tag.c == 0);
}

int main(){
	Treap::Tag f;
	assert(f.b == 1 && f.c == 0);
	Treap tr;
	assert(tr.root == 0 && sz(tr.t) == 1);
	vector<ll> brute;
	tr.insert(1, 3); brute.insert(brute.begin(), 3);
	tr.insert(2, 5); brute.insert(brute.begin() + 1, 5);
	tr.insert(1, 1); brute.insert(brute.begin(), 1);
	tr.insert(2, 2); brute.insert(brute.begin() + 1, 2);
	tr.insert(4, 4); brute.insert(brute.begin() + 3, 4);
	check(tr, brute);
	assert(brute == vector<ll>({1, 2, 3, 4, 5}));
	tr.update(1, 5, Treap::Tag());
	check(tr, brute);
	auto [a, b] = tr.split(tr.root, 0);
	assert(a == 0 && b == tr.root);
	tr.root = tr.merge(a, b);
	auto [c, d] = tr.split(tr.root, 5);
	assert(c == tr.root && d == 0);
	tr.root = tr.merge(c, d);
	auto [e, g] = tr.split(tr.root, 2);
	assert(tr.t[e].size == 2 && tr.t[g].size == 3);
	tr.root = tr.merge(e, g);
	check(tr, brute);
	Treap other;
	other.insert(1, 7);
	// reverse, affine, reverse: overlapping ranges
	tr.reverse(1, 4);
	reverse(brute.begin(), brute.begin() + 4);
	check(tr, brute);
	tr.update(2, 5, {2, 3});
	FOR(i, 1, 4) brute[i] = brute[i] * 2 + 3;
	check(tr, brute);
	tr.reverse(3, 5); reverse(brute.begin() + 2, brute.end());
	check(tr, brute);
	tr.update(1, 3, {3, 1});
	FOR(i, 0, 2) brute[i] = brute[i] * 3 + 1;
	tr.reverse(1, 5); reverse(all(brute));
	check(tr, brute);
	int nodes = sz(tr.t);
	tr.erase(1); brute.erase(brute.begin());
	assert(sz(tr.t) == nodes);
	tr.insert(1, 9); brute.insert(brute.begin(), 9);
	assert(sz(tr.t) == nodes + 1);
	tr.erase(1); brute.erase(brute.begin());
	check(tr, brute);
	tr.erase(sz(brute)); brute.pop_back();
	tr.erase(2); brute.erase(brute.begin() + 1);
	check(tr, brute);
	assert(other.get(1, 1).sum == 7);
	tr.erase(1); tr.erase(1);
	assert(tr.root == 0);
	tr = Treap();
	assert(tr.root == 0 && sz(tr.t) == 1);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	FOR(i, 1, n){
		MintN x; cin >> x;
		tr.insert(i, x);
	}
	FOR(i, 1, q){
		int type, l, r; cin >> type >> l;
		if(type == 0){
			MintN x; cin >> x;
			tr.insert(l + 1, x);
		}else if(type == 1)
			tr.erase(l + 1);
		else{
			cin >> r;
			if(type == 2) tr.reverse(l + 1, r);
			else if(type == 3){
				MintN mb, mc; cin >> mb >> mc;
				tr.update(l + 1, r, {mb, mc});
			}else
				cout << tr.get(l + 1, r).sum << '\n';
		}
	}
}
