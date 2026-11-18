#pragma once
#include "../contest/template.hpp"

// extGcd: a,b>=0; ax+by=g; a,b>=1: |x|<=b/g, |y|<=a/g.
// Recursive, depth <= 92; both O(log). CRT of two:
// m,n>=1; |a|<m, |b|<n; m*n<2^62; none: {-1, -1}.
ll extGcd(ll a, ll b, ll &x, ll &y){
	if(!b){
		x = 1; y = 0;
		return a;
	}
	ll d = extGcd(b, a % b, y, x);
	y -= a / b * x;
	return d;
}
pair<ll, ll> crt(ll a, ll m, ll b, ll n){
	if(n > m)
		swap(a, b), swap(m, n);
	ll x, y, g = extGcd(m, n, x, y);
	if((a - b) % g)
		return {-1, -1};
	ll l = m / g * n;
	x = (b - a) % n * x % n / g * m + a;
	return {x < 0 ? x + l : x, l};
}
