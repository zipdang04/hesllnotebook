#include "../../tex/contents/flow/dinitzwscaling.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int L, R, m; cin >> L >> R >> m;
	int s = L + R, t = L + R + 1;
	Dinic f(L + R + 2);
	FOR(int, i, 0, L - 1) f.addEdge(s, i, 1);
	FOR(int, j, 0, R - 1) f.addEdge(L + j, t, 1);
	vector<int> ea(m), eb(m);
	FOR(int, i, 0, m - 1) {
		cin >> ea[i] >> eb[i];
		f.addEdge(ea[i], L + eb[i], 1);
	}
	cout << f.maxflow(s, t) << '\n';
	FOR(int, i, 0, m - 1)
		if (f.cap(L + R + i) == 0)
			cout << ea[i] << ' ' << eb[i] << '\n';
}
