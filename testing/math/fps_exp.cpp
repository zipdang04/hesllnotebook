#include "../../tex/contents/math/fps.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(fpsExp({}, 1) == vector<MintN>{1});
	assert((fpsExp({}, 4) == vector<MintN>{1, 0, 0, 0}));
	vector<MintN> g = fpsExp({0, 1}, 5), want(5, 1);
	FOR(i, 1, 4)
		want[i] = want[i-1] / i;
	assert(g == want);
	vector<MintN> a{1, 2, 3, 4, 5, 6, 7};
	assert(fpsExp(fpsLog(a, 7), 7) == a);
	assert((fpsExp({0, 2, 3, 4}, 2)
		== vector<MintN>{1, 2}));
	int n; cin >> n;
	a.resize(n);
	for(MintN &x: a)
		cin >> x;
	g = fpsExp(a, n);
	FOR(i, 0, sz(g) - 1)
		cout << (i ? " " : "") << g[i];
	cout << '\n';
}
