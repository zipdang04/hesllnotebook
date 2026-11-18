#pragma once
#include "millerrabin.hpp"

// discreteLog: least k >= 0, 1 <= m <= 3e9, any a, b;
// O(sqrt m); b = 1 gives 0, not the order of a.
// discreteSqrt: p prime < 2^63; O(log^2 p).
ll discreteLog(ll a, ll b, ll m){
	a = (a % m + m) % m, b = (b % m + m) % m;
	if(b == 1 % m)
		return 0;
	ll n = (ll)sqrt(m) + 1, e = 1, f = 1;
	unordered_map<ll, ll> A; A.reserve(n);
	FOR(j, 1LL, n){
		e = f = e * a % m;
		if(e == b)
			return j;
		A[e * b % m] = j;
	}
	if(__gcd(m, e) == __gcd(m, b))
		FOR(i, 2LL, n + 1){
			e = e * f % m;
			if(A.count(e))
				return n * i - A[e];
		}
	return -1;
}
ll discreteSqrt(ll a, ll p){
	a %= p;
	if(a < 0)
		a += p;
	if(a == 0)
		return 0;
	if(powMod(a, (p - 1) / 2, p) != 1)
		return -1;
	if(p % 4 == 3)
		return powMod(a, (p + 1) / 4, p);
	ll s = p - 1, n = 2;
	int r = 0, m;
	while(s % 2 == 0)
		++r, s /= 2;
	while(powMod(n, (p - 1) / 2, p) != p - 1)
		++n;
	ll x = powMod(a, (s + 1) / 2, p);
	ll b = powMod(a, s, p), g = powMod(n, s, p);
	for(;; r = m){
		ll t = b;
		for(m = 0; m < r && t != 1; ++m)
			t = mulMod(t, t, p);
		if(m == 0)
			return x;
		ll gs = powMod(g, 1LL << (r - m - 1), p);
		g = mulMod(gs, gs, p);
		x = mulMod(x, gs, p), b = mulMod(b, g, p);
	}
}
