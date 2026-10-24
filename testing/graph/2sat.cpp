#include "../../tex/contents/graph/2sat.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string p, cnf; int n, m; cin >> p >> cnf >> n >> m;
	TWOSET ts; ts.init(n);
	FOR(int, i, 1, m) {
		int a, b, zero; cin >> a >> b >> zero;
		ts.addEdge(a, b);
	}
	if (!ts.checkSatisfiable()) {
		cout << "s UNSATISFIABLE\n"; return 0;
	}
	cout << "s SATISFIABLE\n" << 'v';
	FOR(int, i, 1, n) cout << ' ' << (ts.getAnswer(i) ? i : -i);
	cout << " 0\n";
}
