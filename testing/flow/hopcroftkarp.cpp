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
	if ((int)(A.size() + B.size()) != k) return 1;
	FOR(int, i, 0, m - 1)
		if (!inA[a[i]] && !inB[b[i]]) return 1;
	cout << k << '\n';
	FOR(int, u, 0, L - 1)
		if (hk.l[u] != -1) cout << u << ' ' << hk.l[u] << '\n';
}
