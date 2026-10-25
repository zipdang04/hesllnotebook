#pragma once
#include "../contest/template.hpp"

ll _pw(ll a, ll e, ll m){
	ll r = 1 % m;
	for (a %= m; e; e >>= 1, a = a * a % m) if (e & 1) r = r * a % m;
	return r;
}
// smallest x >= 0 with a^x = b (mod m), or -1; any a, b >= 0, 1 <= m <= 1e9 (0^0 = 1)
ll dlog(ll a, ll b, ll m){
	a %= m, b %= m;
	ll k = 0, c = 1 % m, g;
	for (; (g = __gcd(a, m)) > 1; k++){ // strip common factors: c a^(x-k) = b (mod m)
		if (b == c) return k;
		if (b % g) return -1;
		b /= g, m /= g, c = c * (a / g) % m;
	}
	if (b == c) return k;
	ll n = sqrtl(m) + 1, t = b, s = _pw(a, n, m);
	unordered_map<ll, ll> mp; // baby steps: b a^j -> largest j
	EFOR(int, j, 0, n) mp[t] = j, t = t * a % m;
	t = c;
	FOR(int, i, 1, n){
		t = t * s % m;
		if (mp.count(t)) return k + i * n - mp[t];
	}
	return -1;
}
// x with x^2 = a (mod p) (Tonelli-Shanks), or -1; p prime <= 1e9, other root is p - x
ll dsqrt(ll a, ll p){
	if ((a %= p) < 2) return a;
	if (_pw(a, (p - 1) / 2, p) != 1) return -1;
	ll q = p - 1, s = 0, z = 2;
	for (; ~q & 1; q >>= 1) s++;
	while (_pw(z, (p - 1) / 2, p) == 1) z++;
	ll x = _pw(a, (q + 1) / 2, p), t = _pw(a, q, p), c = _pw(z, q, p);
	while (t != 1){
		ll i = 0, u = t;
		for (; u != 1; i++) u = u * u % p;
		ll b = c;
		EFOR(int, j, 0, s - i - 1) b = b * b % p;
		x = x * b % p, c = b * b % p, t = t * c % p, s = i;
	}
	return x;
}
