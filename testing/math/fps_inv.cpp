#include "../../tex/contents/math/fps.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(fpsInv({1, -1}, 5) == vector<MintN>(5, 1));
	assert(fpsInv({2}, 1) == vector<MintN>{MintN(2).inv()});
	assert((fpsInv({1, 2, 3, 4, 5}, 2)
		== vector<MintN>{1, -2}));
	vector<MintN> a{2, 3, 5, 7, 11};
	vector<MintN> c = convNtt(a, fpsInv(a, 3));
	c.resize(3);
	assert((c == vector<MintN>{1, 0, 0}));
	int n; cin >> n;
	a.resize(n);
	for(MintN &x: a)
		cin >> x;
	vector<MintN> g = fpsInv(a, n);
	FOR(i, 0, sz(g) - 1)
		cout << (i ? " " : "") << g[i];
	cout << '\n';
}
