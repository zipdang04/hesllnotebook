#pragma once
#include "../contest/template.hpp"

// sum_{i=0}^{n-1} floor((a*i + b) / m), any sign of a, b.
// O(log m), iterative (AtCoder Library's loop).
ll floorSum(ll n, ll m, ll a, ll b){
	// floor division, so that 0 <= a, b < m afterwards
	ll qa = a / m - (a % m < 0), qb = b / m - (b % m < 0);
	ll ans = n * (n - 1) / 2 * qa + n * qb;
	a = a % m + (a % m < 0) * m, b = b % m + (b % m < 0) * m;
	while(true){
		if(a >= m) ans += n * (n - 1) / 2 * (a / m), a %= m;
		if(b >= m) ans += n * (b / m), b %= m;
		ll y = a * n + b; if(y < m) return ans;
		n = y / m, b = y % m; swap(m, a);
	}
}
