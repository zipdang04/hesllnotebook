#pragma once
#include "../contest/template.hpp"

// forest on vertices 0..n-1 with values v (sum over paths); node n = null
struct LCT{
	struct Node{int c[2], p; bool f; ll v, s;};
	int n; vector<Node> t;
	LCT(int n): n(n), t(n + 1, {{n, n}, n, 0, 0, 0}){}
	bool isRoot(int x){ int p = t[x].p; return t[p].c[0] != x && t[p].c[1] != x; } // of its splay
	void pull(int x){ t[x].s = t[t[x].c[0]].s + t[x].v + t[t[x].c[1]].s; } // op must be commutative
	void push(int x){
		if (t[x].f){ swap(t[x].c[0], t[x].c[1]); for (int y: t[x].c) t[y].f ^= 1; t[x].f = 0; }
	}
	void rot(int x){
		int p = t[x].p, g = t[p].p, d = t[p].c[1] == x, b = t[x].c[!d];
		if (!isRoot(p)) t[g].c[t[g].c[1] == p] = x;
		t[x].p = g; t[p].c[d] = b; t[b].p = p; t[x].c[!d] = p; t[p].p = x;
		pull(p); pull(x);
	}
	void splay(int x){
		for (push(x); !isRoot(x); rot(x)){
			int p = t[x].p, g = t[p].p;
			if (!isRoot(p)) push(g);
			push(p); push(x);
			if (!isRoot(p)) rot((t[g].c[1] == p) == (t[p].c[1] == x) ? p : x);
		}
	}
	int access(int x){ // x becomes splay root of the root-x path; returns last jump target
		int last = n;
		for (int y = x; y != n; y = t[y].p) splay(y), t[y].c[1] = last, pull(y), last = y;
		splay(x); return last;
	}
	void makeRoot(int x){ access(x); t[x].f ^= 1; }
	int findRoot(int x){
		access(x);
		while (push(x), t[x].c[0] != n) x = t[x].c[0];
		splay(x); return x;
	}
	bool connected(int u, int v){ return findRoot(u) == findRoot(v); }
	void link(int u, int v){ makeRoot(u); t[u].p = v; } // u, v not connected
	void cut(int u, int v){ makeRoot(u); access(v); t[v].c[0] = t[u].p = n; pull(v); } // edge must exist
	void set(int u, ll x){ access(u); t[u].v = x; pull(u); }
	ll query(int u, int v){ makeRoot(u); access(v); return t[v].s; } // u, v connected
	int lca(int r, int u, int v){ makeRoot(r); access(u); return access(v); } // rooted at r, connected
};
