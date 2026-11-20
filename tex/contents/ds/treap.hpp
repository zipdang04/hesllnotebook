#pragma once
#include "../math/modint.hpp"

// Implicit treap: a sequence, positions 1..size, [l, r]
// closed; O(log n) expected per call. Other ops: edit EDIT.
struct Treap{
	// ---- EDIT: node value, pending tag, two functions ----
	// x -> b * x + c. Tag() must change nothing.
	struct Tag{ MintN b = 1, c = 0; };
	// Node() is the null node 0: size 0, neutral sum.
	struct Node{
		MintN val = 0, sum = 0;
		int size = 0, l = 0, r = 0, pri = 0;
		bool rev = 0;
		Tag tag;
	};
	// size and sum of u from l, val, r (tag not read)
	void pull(int u){
		Node &x = t[u];
		x.size = t[x.l].size + 1 + t[x.r].size;
		x.sum = t[x.l].sum + x.val + t[x.r].sum;
	}
	// t[u] gets f after the tag already in u
	void apply(int u, Tag f){
		if(f.b == 1 && f.c == 0) return;
		Node &x = t[u];
		x.val = f.b * x.val + f.c;
		x.sum = f.b * x.sum + f.c * x.size;
		x.tag = {f.b * x.tag.b, f.b * x.tag.c + f.c};
	}
	// ---- end of EDIT ----
	vector<Node> t;
	int root = 0;
	mt19937 rng;
	Treap(): t(1){}
	int newNode(MintN x){
		int u = sz(t); t.push_back(Node());
		t[u].val = x; t[u].pri = (int)(rng() >> 1);
		pull(u);
		return u;
	}
	// u's children are swapped already; rev, tag go down
	void push(int u){
		Node &x = t[u];
		for(int c: {x.l, x.r}) if(c){
			if(x.rev) swap(t[c].l, t[c].r), t[c].rev ^= 1;
			apply(c, x.tag);
		}
		x.rev = 0; x.tag = Tag();
	}
	pair<int, int> split(int u, int k){
		if(!u) return {0, 0};
		push(u);
		if(t[t[u].l].size >= k){
			auto [a, b] = split(t[u].l, k);
			t[u].l = b; pull(u);
			return {a, u};
		}
		auto [a, b] = split(t[u].r, k - t[t[u].l].size - 1);
		t[u].r = a; pull(u);
		return {u, b};
	}
	int merge(int a, int b){
		if(!a || !b) return a ? a : b;
		if(t[a].pri > t[b].pri){
			push(a); t[a].r = merge(t[a].r, b); pull(a);
			return a;
		}
		push(b); t[b].l = merge(a, t[b].l); pull(b);
		return b;
	}
	void insert(int pos, MintN x){
		auto [a, b] = split(root, pos - 1);
		root = merge(merge(a, newNode(x)), b);
	}
	void erase(int pos){
		auto [a, bc] = split(root, pos - 1);
		auto [b, c] = split(bc, 1); root = merge(a, c);
	}
	void reverse(int l, int r){
		auto [a, bc] = split(root, l - 1);
		auto [b, c] = split(bc, r - l + 1);
		swap(t[b].l, t[b].r); t[b].rev ^= 1;
		root = merge(merge(a, b), c);
	}
	void update(int l, int r, Tag f){
		auto [a, bc] = split(root, l - 1);
		auto [b, c] = split(bc, r - l + 1);
		apply(b, f);
		root = merge(merge(a, b), c);
	}
	Node get(int l, int r){
		auto [a, bc] = split(root, l - 1);
		auto [b, c] = split(bc, r - l + 1);
		Node res = t[b];
		root = merge(merge(a, b), c);
		return res;
	}
};
