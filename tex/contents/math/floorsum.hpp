#pragma once
#include "../contest/template.hpp"

typedef unsigned long long ull;
// sum_{0 <= i < n} floor((a i + b) / m); 0 <= n < 2^32, 1 <= m < 2^32, any a, b
ll floorSum(ll n, ll m, ll a, ll b){
	ll q = a / m - (a % m < 0), p = b / m - (b % m < 0);
	a -= q * m, b -= p * m; // now 0 <= a, b < m; sums wrap mod 2^64
	ull y = (ull)a * n + b, r = (ull)n * (n - 1) / 2 * q + (ull)n * p;
	return y < (ull)m ? r : r + floorSum(y / m, a, m, y % m);
}
