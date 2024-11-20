#include "../../miscellanous/template.hpp"

#define MAX 1000001
#define MOD 1000000007

namespace MillerRabin{
	typedef __int128_t i128;
	constexpr int SMALL_PRIMES[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

	inline ll _power(ll a, ll n, ll mod) {
		ll ans = 1;
		while (n) {
			if (n & 1) ans = (i128)ans * a % mod;
			a = (i128)a * a % mod; n >>= 1;
		}
		return ans;
	}
	inline bool _fermatCheck(ll n, ll a, ll pw, int p2) {
		ll num = _power(a, pw, n);
		if (num == 1 || num == n - 1) return true;	
		FOR(int, i, 1, p2-1) {
			num = (i128) num * num % n;
			if (num == n - 1) return true;
		}
		return false;
	}

	inline bool checkPrime(const ll n) {
		if (n == 2 || n == 3 || n == 5 || n == 7) return true;
		if (n < 11 || (n & 1) == 0) return false;
		ll d = n-1; int p2 = 0;
		while ((d & 1) == 0) d >>= 1, p2++;
		for (int a: SMALL_PRIMES)
			if (n == a) return true;
			else if (not _fermatCheck(n, a, d, p2)) 
				return false;
		return true;
	}
}