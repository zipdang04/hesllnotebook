#include "../../tex/contents/math/floorsum.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(floorSum(0, 5, -7, -9) == 0);
	assert(floorSum(4, 10, 6, 3) == 3);
	// LC limit: sum_{k=1}^{m} floor((m-1)k/m) = m(m+1)/2 - m
	ll big = 1000000000;
	assert(floorSum(big, big, big - 1, big - 1)
		== big * (big + 1) / 2 - big);
	int t; cin >> t;
	while(t--){
		ll n, m, a, b; cin >> n >> m >> a >> b;
		cout << floorSum(n, m, a, b) << '\n';
	}
}
