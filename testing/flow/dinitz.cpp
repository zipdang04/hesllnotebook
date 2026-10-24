#include "../../tex/contents/flow/dinitzwscaling.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int L, R, m; cin >> L >> R >> m;
	int s = L + R, t = L + R + 1;
	Flow f; f.init(L + R + 2, s, t);
	FOR(int, i, 0, L - 1) f.addEdge(s, i, 1);
	FOR(int, j, 0, R - 1) f.addEdge(L + j, t, 1);
	vector<int> ea(m), eb(m);
	FOR(int, i, 0, m - 1) {
		cin >> ea[i] >> eb[i];
		f.addEdge(ea[i], L + eb[i], 1);
	}
	cout << f.calculate() << '\n';
	FOR(int, i, 0, m - 1)
		if (f.capOf(L + R + i) == 0)
			cout << ea[i] << ' ' << eb[i] << '\n';
}
