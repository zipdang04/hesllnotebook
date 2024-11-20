#include "millerrabin.hpp"
randomize;
namespace Pollards{
	typedef __int128_t i128;
	#define sqp1(a, b, mod) (((i128(a)*(a)+(b)))%(mod))
	#define rand (mt()%1'000'000'000 + 1)
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
			if (g == n) x=y=rand, b=rand, g=1;
		}
		vector<ll> tmp1 = rho(g), tmp2 = rho(n / g); 
		ans.insert(ans.end(), tmp1.begin(), tmp1.end());
		ans.insert(ans.end(), tmp2.begin(), tmp2.end());
		return ans;
	}
}