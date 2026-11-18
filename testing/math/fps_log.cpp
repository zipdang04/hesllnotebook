#include "../../tex/contents/math/fps.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	vector<MintN> g = fpsLog({1, -1}, 6), want(6);
	FOR(i, 1, 5)
		want[i] = -MintN(1) / i;
	assert(g == want);
	assert(fpsLog({1}, 1) == vector<MintN>{0});
	assert(fpsLog({1}, 4) == vector<MintN>(4, 0));
	assert((fpsLog({1, 2, 3, 4}, 2)
		== vector<MintN>{0, 2}));
	int n; cin >> n;
	vector<MintN> a(n);
	for(MintN &x: a)
		cin >> x;
	g = fpsLog(a, n);
	FOR(i, 0, sz(g) - 1)
		cout << (i ? " " : "") << g[i];
	cout << '\n';
}
