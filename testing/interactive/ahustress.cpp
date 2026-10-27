#include "../../tex/contents/interactive/ahu.hpp"
mt19937_64 mt;
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }
typedef vector<vector<int>> G;
G gen(int n){ // shape: 0 random, 1 path, 2 star, 3 caterpillar-ish, 4 deep random (parent near i)
	G g(n); int s = rnd(0, 4);
	EFOR(int, i, 1, n){
		int p = s == 0 ? rnd(0, i - 1) : s == 1 ? i - 1 : s == 2 ? 0 : s == 3 ? (i % 2 ? i - 1 : max(0, i - 2)) : rnd(max(0, i - 2), i - 1);
		g[p].push_back(i); g[i].push_back(p);
	}
	return g;
}
G relabel(const G &g, vector<int> &p){ // p: old -> new, shuffled adjacency
	int n = g.size(); p.resize(n); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), mt);
	G h(n); EFOR(int, v, 0, n) for (int w: g[v]) h[p[v]].push_back(p[w]);
	for (auto &x: h) shuffle(x.begin(), x.end(), mt);
	return h;
}
bool iso(const G &a, const G &b, int ra, int rb){ // brute: try all bijections with ra -> rb (rb < 0: any)
	int n = a.size(); vector<int> p(n); iota(p.begin(), p.end(), 0);
	vector<vector<char>> A(n, vector<char>(n)), B = A;
	EFOR(int, v, 0, n) for (int w: a[v]) A[v][w] = 1;
	EFOR(int, v, 0, n) for (int w: b[v]) B[v][w] = 1;
	do{
		if (ra >= 0 && p[ra] != rb) continue;
		bool ok = true;
		EFOR(int, v, 0, n) EFOR(int, w, 0, n) if (A[v][w] != B[p[v]][p[w]]) ok = false;
		if (ok) return true;
	} while (next_permutation(p.begin(), p.end()));
	return false;
}
int main(){
	for (int seed = 0; seed < 3000; seed++){
		mt.seed(seed);
		int n = rnd(1, 7); G a = gen(n), b = gen(n), c; vector<int> p;
		if (rnd(0, 1)) b = relabel(a, p); // often isomorphic
		AHU t; bool ok = true;
		// rooted: ids of (a, ra) and (b, rb) equal iff isomorphic with roots matched
		int ra = rnd(0, n - 1), rb = rnd(0, n - 1);
		int x = t.rooted(a, ra); vector<int> ia = t.id; int y = t.rooted(b, rb);
		if ((x == y) != iso(a, b, ra, rb)) ok = false;
		// id[v] of every vertex = class of its subtree: compare with the same tree rerooted at a relabelled root
		int r0 = rnd(0, n - 1); G h = relabel(a, p);
		t.rooted(a, r0); vector<int> i1 = t.id; t.rooted(h, p[r0]);
		EFOR(int, v, 0, n) if (i1[v] != t.id[p[v]]) ok = false; // v's subtree is the same in the relabelled tree
		// unrooted
		if ((t.unrooted(a) == t.unrooted(b)) != iso(a, b, -1, -1)) ok = false;
		if (t.unrooted(a) != t.unrooted(h)) ok = false;
		if (!ok){ printf("FAIL seed %d n %d\n", seed, n); return 1; }
	}
	// large path / star: iterative, no stack overflow, ids consistent
	int N = 500000; G g(N);
	EFOR(int, i, 1, N) g[i - 1].push_back(i), g[i].push_back(i - 1);
	AHU t; int u1 = t.unrooted(g); G h(N);
	EFOR(int, i, 0, N - 1) h[i].push_back(i + 1), h[i + 1].push_back(i);
	assert(t.unrooted(h) == u1 && t.rooted(g, 0) != t.rooted(g, 1));
	// large random trees (n = 2e5): relabelled copy matches; a leaf moved elsewhere changes the degree multiset -> differs
	int twoCentres = 0;
	for (int seed = 0; seed < 8; seed++){
		mt.seed(1000 + seed); int n = 200000;
		G a(n); EFOR(int, i, 1, n){ int p = seed % 2 ? rnd(0, i - 1) : rnd(max(0, i - 3), i - 1); a[p].push_back(i); a[i].push_back(p); }
		auto far = [&](int s, vector<int> &d){ d.assign(n, -1); d[s] = 0; vector<int> q{s};
			for (int i = 0; i < (int)q.size(); i++) for (int w: a[q[i]]) if (d[w] < 0) d[w] = d[q[i]] + 1, q.push_back(w);
			return q.back(); };
		vector<int> d; int x = far(0, d); int y = far(x, d); twoCentres += d[y] % 2;
		vector<int> p; G h = relabel(a, p);
		AHU s; int ua = s.unrooted(a);
		if (s.unrooted(h) != ua){ printf("FAIL large seed %d\n", seed); return 1; }
		// b: move leaf n-1 (always a leaf) under a vertex of max degree; changes the degree multiset unless already attached there
		G b = a; int par = b[n - 1][0], m = 0; EFOR(int, v, 0, n - 1) if (b[v].size() > b[m].size()) m = v;
		if (m != par && b[par].size() != b[m].size() + 1){
			b[par].erase(find(b[par].begin(), b[par].end(), n - 1)); b[n - 1] = {m}; b[m].push_back(n - 1);
			if (s.unrooted(b) == ua){ printf("FAIL large non-iso seed %d\n", seed); return 1; }
		}
	}
	assert(twoCentres > 0 && twoCentres < 8);
	puts("ok");
}
