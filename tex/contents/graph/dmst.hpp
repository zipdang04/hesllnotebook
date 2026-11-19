#pragma once
#include "../ds/rollbackdsu.hpp"

// Min rooted arborescence: skew heaps + rollback DSU.
// 1..n, g[u]={v,w}, O((V + E) log E), self-loops skipped.
// Recursive merge depth <= E. LLONG_MAX: unreachable.
struct DirectedMST{
	struct Edge{ int a, b; ll w; };
	struct Node{ Edge key; int l, r; ll delta; };
	ll cost = 0;
	vector<int> par;
	vector<Node> heap;
	DirectedMST(const vector<vector<pair<int, ll>>> &g,
			int r): par(sz(g)), heap(1){
		int n = sz(g) - 1; RollbackDsu dsu(n + 1);
		vector<int> head(n + 1), seen(n + 1, -1), path(n + 1);
		vector<Edge> Q(n + 1), in(n + 1);
		deque<tuple<int, int, vector<Edge>>> cycs;
		FOR(u, 1, n)
			for(auto [v, w]: g[u]) if(v != u){
				heap.push_back({{u, v, w}, 0, 0, 0});
				head[v] = merge(head[v], sz(heap) - 1);
			}
		seen[r] = r;
		FOR(s, 1, n){
			int u = s, qi = 0, w;
			while(seen[u] < 0){
				if(!head[u]){ cost = LLONG_MAX; return; }
				prop(head[u]); Edge e = heap[head[u]].key;
				heap[head[u]].delta -= e.w; pop(head[u]);
				Q[qi] = e, path[qi++] = u, seen[u] = s;
				cost += e.w, u = dsu.find(e.a);
				if(seen[u] == s){
					int cyc = 0, end = qi, t = dsu.time();
					do{
						w = path[--qi];
						cyc = merge(cyc, head[w]);
					}while(dsu.unite(u, w));
					u = dsu.find(u), head[u] = cyc, seen[u] = -1;
					cycs.push_front({u, t,
						vector<Edge>(Q.begin() + qi, Q.begin() + end)});
				}
			}
			FOR(i, 0, qi - 1) in[dsu.find(Q[i].b)] = Q[i];
		}
		for(auto &[u, t, comp]: cycs){
			dsu.rollback(t); Edge inEdge = in[u];
			for(auto &e: comp) in[dsu.find(e.b)] = e;
			in[dsu.find(inEdge.b)] = inEdge;
		}
		FOR(i, 1, n) par[i] = in[i].a;
	}
	void prop(int a){
		ll delta = heap[a].delta; heap[a].key.w += delta;
		if(heap[a].l) heap[heap[a].l].delta += delta;
		if(heap[a].r) heap[heap[a].r].delta += delta;
		heap[a].delta = 0;
	}
	int merge(int a, int b){
		if(!a || !b) return a ? a : b;
		prop(a); prop(b);
		if(heap[a].key.w > heap[b].key.w) swap(a, b);
		int t = merge(b, heap[a].r);
		heap[a].r = heap[a].l; heap[a].l = t; return a;
	}
	void pop(int &a){
		prop(a); a = merge(heap[a].l, heap[a].r);
	}
};
