#pragma once
#include "../contest/template.hpp"

// Link-cut tree: a dynamic forest, vertices 1..n, node 0 =
// null. Amortised O(log n) per call, no recursion.
struct LinkCut{
	// ---- EDIT: node value, pending tag, two functions ----
	// x -> x + add. Tag() must change nothing.
	struct Tag{ ll add = 0; };
	// Node() is node 0 (null): neutral, never written.
	struct Node{
		ll val = 0, sum = 0; Tag tag;
		int size = 0, par = 0, ch[2] = {0, 0}; bool rev = 0;
	};
	// u's splay subtree: ch[0], u, ch[1] in path order
	void pull(int u){
		Node &x = t[u], &l = t[x.ch[0]], &r = t[x.ch[1]];
		x.size = l.size + 1 + r.size;
		x.sum = l.sum + x.val + r.sum;
	}
	// t[u] gets f after the tag already in u
	void apply(int u, Tag f){
		t[u].val += f.add; t[u].sum += f.add * t[u].size;
		t[u].tag.add += f.add;
	}
	// ---- end of EDIT ----
	vector<Node> t;
	LinkCut(int n): t(n){
		FOR(u, 1, n - 1)
			t[u].size = 1;
	}
	bool isRoot(int u){
		int p = t[u].par;
		return t[p].ch[0] != u && t[p].ch[1] != u;
	}
	void flip(int u){
		swap(t[u].ch[0], t[u].ch[1]); t[u].rev ^= 1;
	}
	void push(int u){
		for(int c: t[u].ch) if(c){
			if(t[u].rev)
				flip(c);
			apply(c, t[u].tag);
		}
		t[u].rev = 0; t[u].tag = Tag();
	}
	void rotate(int x){
		int p = t[x].par, g = t[p].par, d = t[p].ch[1] == x;
		if(!isRoot(p))
			t[g].ch[t[g].ch[1] == p] = x;
		t[x].par = g;
		int c = t[x].ch[!d]; t[p].ch[d] = c;
		if(c)
			t[c].par = p;
		t[x].ch[!d] = p; t[p].par = x; pull(p);
	}
	void splay(int x){
		while(!isRoot(x)){
			int p = t[x].par, g = t[p].par;
			if(!isRoot(p))
				push(g);
			push(p); push(x);
			if(!isRoot(p)){
				bool same = (t[g].ch[0] == p) == (t[p].ch[0] == x);
				rotate(same ? p : x);
			}
			rotate(x);
		}
		push(x); pull(x);
	}
	int access(int u){
		int last = 0;
		for(int y = u; y; y = t[y].par)
			splay(y), t[y].ch[1] = last, pull(y), last = y;
		splay(u);
		return last;
	}
	void makeRoot(int u){ access(u); flip(u); }
	bool connected(int u, int v){
		access(u); access(v);
		return u == v || t[u].par;
	}
	int lca(int u, int v){ access(u); return access(v); }
	void link(int u, int v){ makeRoot(u); t[u].par = v; }
	void cut(int u, int v){
		makeRoot(u); access(v);
		t[v].ch[0] = t[u].par = 0; pull(v);
	}
	void update(int u, ll x){
		access(u); t[u].val = x; pull(u);
	}
	void update(int u, int v, Tag f){
		makeRoot(u); access(v); apply(v, f);
	}
	Node get(int u, int v){
		makeRoot(u); access(v);
		return t[v];
	}
};
