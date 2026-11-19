#include "../../tex/contents/math/fwht.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	// i & j over {0, 1, 2} x {0, 1, 2}, padded to 4
	vector<MintN> c = convBit<998244353>({1, 1, 1},
		{1, 1, 1}, '&');
	assert((c == vector<MintN>{7, 1, 1, 0}));
	int n; cin >> n;
	vector<MintN> a(1 << n), b(1 << n);
	for(MintN &x: a) cin >> x;
	for(MintN &x: b) cin >> x;
	c = convBit(a, b, '&');
	FOR(i, 0, sz(c) - 1)
		cout << c[i] << " \n"[i == sz(c) - 1];
}
