#pragma once
#include "millerrabin.hpp"

namespace Pollards{
	typedef __int128_t i128;
	inline mt19937_64 _rng(chrono::steady_clock::now().time_since_epoch().count());
	inline ll sqp1(ll a, ll b, ll mod) { return (ll)(((i128)a * a + b) % mod); }
	inline ll _rand() { return _rng() % 1'000'000'000 + 1; }
	inline vector<ll> rho(ll n) {
		if (n == 1) return {};
		if (MillerRabin::checkPrime(n)) return {n};
		vector<ll> ans;
		if (n % 2 == 0) {
			while (n % 2 == 0) ans.push_back(2), n /= 2;
			vector<ll> others = rho(n);
			ans.insert(ans.end(), others.begin(), others.end());
			return ans;
		}
		ll x = 2, y = 2, g = 1, b = 1;
		while (g == 1) {
			x=sqp1(x,b,n); y=sqp1(y,b,n); y=sqp1(y,b,n);
			g = __gcd(abs(x-y), n);
			if (g == n) x=y=_rand(), b=_rand(), g=1;
		}
		vector<ll> tmp1 = rho(g), tmp2 = rho(n / g); 
		ans.insert(ans.end(), tmp1.begin(), tmp1.end());
		ans.insert(ans.end(), tmp2.begin(), tmp2.end());
		return ans;
	}
}