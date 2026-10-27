#pragma once
#include "../contest/template.hpp"

// sequence a[1..size()]; policy M as in LazySeg: S, F, op, e, mp(f, x) = f(x), cp(f, g) = f after g, id
// node: v, s (own value, subtree aggregate) already include lz; lz, rv still owed to the children
template<class M> struct Treap{
	using S = typename M::S; using F = typename M::F;
	struct Node{int l, r, sz; unsigned pr; bool rv; S v, s; F lz;};
	vector<Node> t; int root = 0; mt19937 rng{12345};
	Treap(const vector<S>& a = {}): t(1){ // position i = a[i-1]
		t[0].s = M::e(); for (S x: a) root = _merge(root, _new(x));
	}
	int _new(S x){ t.push_back({0, 0, 1, (unsigned)rng(), 0, x, x, M::id()}); return t.size() - 1; }
	void _pull(int x){
		Node& n = t[x]; n.sz = t[n.l].sz + 1 + t[n.r].sz;
		n.s = M::op(M::op(t[n.l].s, n.v), t[n.r].s);
	}
	void _put(int x, F f, bool rv){
		if (!x) return;
		Node& n = t[x]; n.v = M::mp(f, n.v); n.s = M::mp(f, n.s); n.lz = M::cp(f, n.lz);
		if (rv) swap(n.l, n.r), n.rv ^= 1;
	}
	void _push(int x){
		Node& n = t[x]; _put(n.l, n.lz, n.rv); _put(n.r, n.lz, n.rv); n.lz = M::id(); n.rv = 0;
	}
	pair<int, int> _split(int x, int k){ // {first k elements, rest}
		if (!x) return {0, 0};
		_push(x);
		if (t[t[x].l].sz >= k){ auto [a, b] = _split(t[x].l, k); t[x].l = b; _pull(x); return {a, x}; }
		auto [a, b] = _split(t[x].r, k - t[t[x].l].sz - 1); t[x].r = a; _pull(x); return {x, b};
	}
	int _merge(int a, int b){
		if (!a || !b) return a | b;
		if (t[a].pr > t[b].pr){ _push(a); t[a].r = _merge(t[a].r, b); _pull(a); return a; }
		_push(b); t[b].l = _merge(a, t[b].l); _pull(b); return b;
	}
	template<class G> void _range(int L, int R, G g){ // g(root of a[L..R]), may replace it
		auto [a, bc] = _split(root, L - 1); auto [b, c] = _split(bc, R - L + 1);
		g(b); root = _merge(_merge(a, b), c);
	}
	int size(){ return t[root].sz; }
	void insert(int i, S x){ // new a[i] = x
		auto [a, b] = _split(root, i - 1); root = _merge(_merge(a, _new(x)), b);
	}
	void erase(int i){ _range(i, i, [&](int& b){ b = 0; }); }
	void reverse(int L, int R){ _range(L, R, [&](int b){ _put(b, M::id(), 1); }); }
	void apply(int L, int R, F f){ _range(L, R, [&](int b){ _put(b, f, 0); }); } // a[i] = f(a[i]), L <= i <= R
	S query(int L, int R){ S r; _range(L, R, [&](int b){ r = t[b].s; }); return r; } // e() if L = R + 1
};
