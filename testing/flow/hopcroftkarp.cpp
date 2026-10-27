#include "../../tex/contents/flow/hopcroftkarp.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int L, R, m; cin >> L >> R >> m;
	HopcroftKarp hk(L, R);
	vector<int> a(m), b(m);
	FOR(int, i, 0, m - 1) {
		cin >> a[i] >> b[i];
		hk.addEdge(a[i], b[i]);
	}
	int k = hk.run();
	auto [A, B] = hk.cover();
	vector<char> inA(L), inB(R);
	for (int x : A) inA[x] = 1;
	for (int x : B) inB[x] = 1;
	HopcroftKarp w(L, R); // warm start: run on half the edges, add the rest, run again
	FOR(int, i, 0, m / 2 - 1) w.addEdge(a[i], b[i]);
	int part = w.run();
	FOR(int, i, m / 2, m - 1) w.addEdge(a[i], b[i]);
	// test.sh ignores the exit code, so returning before printing makes the checker see WA
	if (part + w.run() != k) return 1;
	if ((int)(A.size() + B.size()) != k) return 1;
	FOR(int, i, 0, m - 1)
		if (!inA[a[i]] && !inB[b[i]]) return 1;
	cout << k << '\n';
	FOR(int, u, 0, L - 1)
		if (hk.l[u] != -1) cout << u << ' ' << hk.l[u] << '\n';
}
