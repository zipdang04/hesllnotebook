#include "../../tex/contents/math/fwht.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	// sizes 1 and 3 pad to 4
	vector<MintN> c = convBit<998244353>({2}, {1, 2, 3}, '^');
	assert((c == vector<MintN>{2, 4, 6, 0}));
	c = convBit<998244353>({1, 1, 1}, {1, 1, 1}, '|');
	assert((c == vector<MintN>{1, 3, 3, 2}));
	int n; cin >> n;
	vector<MintN> a(1 << n), b(1 << n);
	for(MintN &x: a) cin >> x;
	for(MintN &x: b) cin >> x;
	c = convBit(a, b, '^');
	FOR(i, 0, sz(c) - 1)
		cout << c[i] << " \n"[i == sz(c) - 1];
}
