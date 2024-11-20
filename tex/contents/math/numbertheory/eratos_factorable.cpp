#include "../../miscellanous/template.hpp"

// remember to call sieve() before any factorize()
namespace Eratos_Factorable {
	constexpr ll MAX = 100000000;
	constexpr ll EST = 53000000; // 1.1*MAX/ln(MAX)
	int smallDiv[MAX + 1] = {};
	int primes[EST], cntPrime = 0;
	inline void sieve(int size = MAX) {
		memset(smallDiv, false, sizeof(smallDiv));
		primes[++cntPrime] = 2; primes[++cntPrime] = 3;
		for (int i=2; i<=size; i += 2) smallDiv[i] = 2;
		for (int i=3; i<=size; i += 6) smallDiv[i] = 3;
		for (int mul = 1; 6 * mul - 1 <= size; mul++) {
			bool pass = false;
			for(int i:{6*mul-1,6*mul+1})if(!smallDiv[i]){
				primes[++cntPrime] = i; smallDiv[i] = i;
				for (ll j = 1ll*i*i; j <= size; j += i*2)
					if (not smallDiv[j]) smallDiv[j] = i;
				if (1ll * i * i > size) pass = true;
			}
			if (pass) break;
		}
	}
	inline vector<int> factorize(ll number) {
		vector<int> ans;
		while (number > 1) {
			int d = smallDiv[number];
			while (number % d == 0)
				ans.push_back(d), number /= d;
		}
		return ans;
	}
}
