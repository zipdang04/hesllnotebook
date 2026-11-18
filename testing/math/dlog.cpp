#include "../../tex/contents/math/dlog.hpp"
#include <cassert>
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	assert(discreteLog(5, 0, 1) == 0);
	assert(discreteLog(2, 1, 7) == 0);
	assert(discreteLog(0, 0, 10) == 1);
	assert(discreteLog(0, 1, 10) == 0);
	assert(discreteLog(0, 5, 10) == -1);
	assert(discreteLog(2, 8, 10) == 3);
	assert(discreteLog(2, 6, 10) == 4);
	assert(discreteLog(2, 3, 7) == -1);
	assert(discreteLog(-1, 4, 5) == 1);
	assert(discreteLog(3, -1, 7) == 3);
	ll m = 3000000000LL, k = 123456789;
	ll b = powMod(7, k, m), r = discreteLog(7, b, m);
	assert(0 <= r && r <= k && powMod(7, r, m) == b);
	int t; cin >> t;
	while(t--){
		ll x, y; cin >> x >> y >> m;
		cout << discreteLog(x, y, m) << '\n';
	}
}
