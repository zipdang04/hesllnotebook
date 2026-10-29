#include "../../tex/contents/graph/manhattanmst.hpp"

int main(){
	int n; scanf("%d", &n);
	vector<ll> x(n), y(n);
	FOR(int, i, 0, n - 1) scanf("%lld %lld", &x[i], &y[i]);
	ManhattanMST m(x, y);
	string s = to_string(m.cost) + "\n";
	for (auto [w, u, v]: m.edges) s += to_string(u) + " " + to_string(v) + "\n";
	fputs(s.c_str(), stdout);
}
