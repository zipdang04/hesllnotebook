#pragma once
#include "../math/modint.hpp"

// Iterative lazy segment tree (ACL), O(log n) per call.
// a's indices, closed [l, r]; edit EDIT.
struct LazySeg{
	// ---- EDIT: node value, pending tag, two functions ----
	struct Node{ MintN sum = 0; int len = 0; };
	// x -> b * x + c. Tag() must change nothing.
	struct Tag{ MintN b = 1, c = 0; };
	// Node() is neutral; x is left of y.
	Node combine(Node x, Node y){
		return {x.sum + y.sum, x.len + y.len};
	}
	// node[id] gets t after the tag already in id
	void apply(int id, Tag t){
		node[id].sum = t.b * node[id].sum + t.c * node[id].len;
		tag[id] = {t.b * tag[id].b, t.b * tag[id].c + t.c};
	}
	// ---- end of EDIT ----
	int n, h, m; // size, height, leaves m = 2^h >= n
	vector<Node> node; vector<Tag> tag;
	LazySeg(const vector<Node> &a): n(sz(a)), h(0){
		for(m = 1; m < n; m <<= 1) h++;
		node.assign(2 * m, Node()); tag.assign(2 * m, Tag());
		FOR(i, 0, n - 1) node[m + i] = a[i];
		REV(id, m - 1, 1) pull(id);
	}
	void pull(int id){
		node[id] = combine(node[2 * id], node[2 * id + 1]);
	}
	void push(int id){
		apply(2 * id, tag[id]); apply(2 * id + 1, tag[id]);
		tag[id] = Tag();
	}
	void push(int l, int r){
		REV(i, h, 1) if(l & ((1 << i) - 1)) push(l >> i);
		REV(i, h, 1) if(r & ((1 << i) - 1)) push((r - 1) >> i);
	}
	void update(int l, int r, Tag t){
		l += m; r += m + 1;
		push(l, r);
		int a = l, b = r;
		for(; a < b; a >>= 1, b >>= 1){
			if(a & 1) apply(a++, t); if(b & 1) apply(--b, t);
		}
		FOR(i, 1, h){
			if(((l >> i) << i) != l) pull(l >> i);
			if(((r >> i) << i) != r) pull((r - 1) >> i);
		}
	}
	Node get(int l, int r){
		l += m; r += m + 1;
		push(l, r);
		Node sml, smr;
		for(; l < r; l >>= 1, r >>= 1){
			if(l & 1) sml = combine(sml, node[l++]);
			if(r & 1) smr = combine(node[--r], smr);
		}
		return combine(sml, smr);
	}
};
