#include "../../tex/contents/graph/euler.hpp"

int main(){
	mt19937 rng(12345);
	auto rnd = [&](int a, int b){return (int)(rng() % (b - a + 1)) + a;};
	Euler E; vector<int> vs, es;
	int yes = 0, no = 0, circ = 0;
	FOR(int, it, 0, 199999){
		bool dir = it & 1; int n = rnd(1, 5), m = rnd(0, 6);
		E.init(n, dir); vector<pair<int, int>> ed;
		FOR(int, i, 0, m - 1){int u = rnd(0, n - 1), v = rnd(0, n - 1); ed.push_back({u, v}); E.addEdge(u, v);}
		// brute: exhaustive trail search from every start; circuit = some closed trail exists
		bool trail = false, circuit = false; vector<int> used(m);
		function<void(int, int, int)> go = [&](int s, int x, int c){
			if (c == m){trail = true; if (x == s) circuit = true; return;}
			FOR(int, i, 0, m - 1) if (!used[i]){
				auto [a, b] = ed[i];
				if (a == x){used[i] = 1; go(s, b, c + 1); used[i] = 0;}
				if (!dir && b == x && a != b){used[i] = 1; go(s, a, c + 1); used[i] = 0;}
			}
		};
		FOR(int, s, 0, n - 1) go(s, s, 0);
		bool ok = E.solve(vs, es);
		bool bad = ok != trail;
		if (ok && !bad){
			bad |= (int)vs.size() != m + 1 || (int)es.size() != m;
			vector<int> seen(m);
			if (!bad) FOR(int, i, 0, m - 1){
				int e = es[i];
				if (e < 0 || e >= m || seen[e]++){bad = true; break;}
				auto [a, b] = ed[e]; int x = vs[i], y = vs[i + 1];
				if (!((a == x && b == y) || (!dir && a == y && b == x))) bad = true;
			}
			if (!bad && (vs[0] == vs.back()) != circuit) bad = true;
			circ += circuit;
		}
		(ok ? yes : no)++;
		if (bad){
			printf("FAIL dir=%d n=%d m=%d ok=%d trail=%d circuit=%d\n", dir, n, m, ok, trail, circuit);
			for (auto [a, b]: ed) printf("%d %d\n", a, b);
			return 1;
		}
	}
	printf("ok yes=%d no=%d circuits=%d\n", yes, no, circ);
}
