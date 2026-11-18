#include "../../tex/contents/math/bigint.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	Bigint zero = Bigint(-3) + 3;
	assert(zero.d.empty() && !zero.neg);
	Bigint borrow = Bigint("-1000000000") + 1;
	assert(borrow.str() == "-999999999" && sz(borrow.d) == 1);
	int t; cin >> t;
	FOR(i, 1, t){
		Bigint a, b; cin >> a >> b;
		assert((a + b) - b == a);
		cout << a + b << '\n';
	}
}
