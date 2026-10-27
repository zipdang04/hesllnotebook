#pragma once
#include "../contest/template.hpp"

// a x + b y = g = gcd(a, b) (g may be < 0 for negative inputs, |g| = gcd)
ll extGcd(ll a, ll b, ll &x, ll &y){
	if (!b) return x = 1, y = 0, a;
	ll g = extGcd(b, a % b, y, x);
	return y -= a / b * x, g;
}
// x = a (mod m), x = b (mod n), m, n >= 1 -> {x, lcm(m, n)} with 0 <= x < lcm, or {-1, -1}
pair<ll, ll> crt(ll a, ll m, ll b, ll n){
	a = (a % m + m) % m, b = (b % n + n) % n;
	ll x, y, g = extGcd(m, n, x, y), k = n / g;
	if ((b - a) % g) return {-1, -1};
	ll t = (__int128)((b - a) / g) * x % k;
	return {a + (__int128)m * (t < 0 ? t + k : t), m * k};
}
