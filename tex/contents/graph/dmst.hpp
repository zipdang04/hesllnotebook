#pragma once
#include "../ds/rollbackdsu.hpp"

// Edmonds / Tarjan with lazy skew heaps; any weights, multi-edges and self-loops ok
struct DMST{
	struct Edge{int a, b; ll w;};
	struct Node{Edge key; Node *l, *r; ll d;};
	int n; vector<Edge> es;
	DMST(int n): n(n) {}
	void addEdge(int a, int b, ll w){ es.push_back({a, b, w}); }
	static void prop(Node *a){ a->key.w += a->d; for (Node *c: {a->l, a->r}) if (c) c->d += a->d; a->d = 0; }
	static Node *merge(Node *a, Node *b){
		if (!a || !b) return a ? a : b;
		prop(a), prop(b);
		if (a->key.w > b->key.w) swap(a, b);
		swap(a->l, (a->r = merge(b, a->r)));
		return a;
	}
	// {cost, par} with par[r] = -1; {-1, {}} if some vertex is unreachable from r
	pair<ll, vector<int>> solve(int r){
		RollbackDSU uf(n); vector<Node> pool; pool.reserve(es.size());
		vector<Node*> heap(n);
		for (Edge e: es) pool.push_back({e, 0, 0, 0}), heap[e.b] = merge(heap[e.b], &pool.back());
		ll res = 0; vector<int> seen(n, -1), path(n), par(n);
		seen[r] = r;
		vector<Edge> Q(n), in(n, {-1, -1, 0});
		deque<tuple<int, int, vector<Edge>>> cycs;
		EFOR(int, s, 0, n){
			int u = s, qi = 0, w;
			while (seen[u] < 0){
				if (!heap[u]) return {-1, {}};
				prop(heap[u]); Edge e = heap[u]->key; // cheapest edge into u; later ones cost w - e.w
				heap[u]->d -= e.w; prop(heap[u]); heap[u] = merge(heap[u]->l, heap[u]->r);
				Q[qi] = e, path[qi++] = u, seen[u] = s;
				res += e.w, u = uf.find(e.a);
				if (seen[u] == s){ // cycle: contract it
					Node *cyc = 0; int end = qi, t = uf.time();
					do cyc = merge(cyc, heap[w = path[--qi]]); while (uf.join(u, w));
					u = uf.find(u), heap[u] = cyc, seen[u] = -1;
					cycs.push_front({u, t, {&Q[qi], &Q[end]}});
				}
			}
			EFOR(int, i, 0, qi) in[uf.find(Q[i].b)] = Q[i];
		}
		for (auto &[u, t, comp]: cycs){ // expand cycles, newest first
			uf.rollback(t); Edge inE = in[u];
			for (auto &e: comp) in[uf.find(e.b)] = e;
			in[uf.find(inE.b)] = inE;
		}
		EFOR(int, i, 0, n) par[i] = in[i].a;
		return {res, par};
	}
};
