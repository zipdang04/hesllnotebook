#pragma once
#include "../contest/template.hpp"

// mulMod, powMod: a, b, n >= 0, 1 <= m < 2^63; r in 0..m-1.
// isPrime: any ll; 7 powMods, O(log n) mulMods each.
ll mulMod(ll a, ll b, ll m){ return (__int128)a * b % m; }
ll powMod(ll a, ll n, ll m){
	ll r = 1 % m;
	for(; n; n >>= 1, a = mulMod(a, a, m))
		if(n & 1) r = mulMod(r, a, m);
	return r;
}
bool isPrime(ll n){
	if(n < 2) return false; if(n < 4) return true;
	if(n % 2 == 0) return false;
	int twos = __builtin_ctzll(n - 1);
	ll odd = (n - 1) >> twos;
	ll bases[] = {2, 325, 9'375, 28'178, 450'775, 9'780'504,
		1'795'265'022};
	for(ll a: bases){
		if(a % n == 0) continue; // says nothing
		ll x = powMod(a, odd, n);
		if(x == 1 || x == n - 1) continue;
		FOR(i, 1, twos - 1){
			x = mulMod(x, x, n);
			if(x == n - 1) break;
		}
		if(x != n - 1) return false;
	}
	return true;
}
