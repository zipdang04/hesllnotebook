#pragma once
#include "../contest/template.hpp"

// Persistent segment tree, point set; O(log n) time and
// nodes per call. Version 0 = a; positions = indices of a,
// ranges [l, r] closed. Recursive, depth O(log n).
struct PerSeg{
	// ---- EDIT: node value, one function ----
	struct Node{ ll sum = 0; };
	// Node() is neutral; x is left of y.
	Node combine(Node x, Node y){
		return {x.sum + y.sum};
	}
	// ---- end of EDIT ----
	int n;
	vector<Node> node;
	// children of a node; root of each version
	vector<int> lc, rc, root;
	PerSeg(const vector<Node> &a): n(sz(a)){
		root.push_back(build(0, n - 1, a));
	}
	int newNode(Node x, int l, int r){
		node.push_back(x); lc.push_back(l); rc.push_back(r);
		return sz(node) - 1;
	}
	int build(int lo, int hi, const vector<Node> &a){
		if(lo == hi) return newNode(a[lo], 0, 0);
		int mid = (lo + hi) / 2;
		int l = build(lo, mid, a), r = build(mid + 1, hi, a);
		return newNode(combine(node[l], node[r]), l, r);
	}
	int update(int id, int lo, int hi, int p, Node x){
		if(lo == hi) return newNode(x, 0, 0);
		int mid = (lo + hi) / 2, l = lc[id], r = rc[id];
		if(p <= mid) l = update(l, lo, mid, p, x);
		else r = update(r, mid + 1, hi, p, x);
		return newNode(combine(node[l], node[r]), l, r);
	}
	Node get(int id, int lo, int hi, int l, int r){
		if(r < lo || hi < l) return Node();
		if(l <= lo && hi <= r) return node[id];
		int mid = (lo + hi) / 2;
		Node x = get(lc[id], lo, mid, l, r),
			y = get(rc[id], mid + 1, hi, l, r);
		return combine(x, y);
	}
	// new version = version v with a[p] = x; returns its id
	int update(int v, int p, Node x){
		root.push_back(update(root[v], 0, n - 1, p, x));
		return sz(root) - 1;
	}
	Node get(int v, int l, int r){
		return get(root[v], 0, n - 1, l, r);
	}
};
