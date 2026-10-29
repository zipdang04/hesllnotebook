#include "../../tex/contents/graph/scc.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string p, cnf; int n, m; cin >> p >> cnf >> n >> m; TwoSat ts(n);
	FOR(int, i, 0, m - 1){ int a, b, z; cin >> a >> b >> z; ts.either(a > 0 ? a - 1 : a, b > 0 ? b - 1 : b); }
	if (!ts.solve()){ cout << "s UNSATISFIABLE\n"; return 0; }
	cout << "s SATISFIABLE\nv";
	FOR(int, i, 0, n - 1) cout << ' ' << (ts.val[i] ? i + 1 : -i - 1);
	cout << " 0\n";
}
