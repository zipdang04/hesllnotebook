#include "../../tex/contents/interactive/hamming.hpp"

mt19937_64 mt;
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(mt); }

int main(){
	FOR(int, seed, 0, 49){
		mt.seed(seed);
		for (int n = 4; n <= 256; n *= 2){ // n = 256 -> 247 data bits
			Hamming h(n); int k = h.pos.size();
			assert(k == n - log2(n) - 1);
			vector<int> d(k);
			for (int &x: d) x = rnd(0, 1);
			if (seed == 0) fill(d.begin(), d.end(), 0);
			if (seed == 1) fill(d.begin(), d.end(), 1);
			auto c = h.encode(d);
			bool ok = (int)c.size() == n;
			vector<int> c2 = c, out;
			ok = ok && h.decode(c2, out) == 0 && out == d && c2 == c;
			EFOR(int, a, 0, n){
				c2 = c; c2[a] ^= 1;
				ok = ok && h.decode(c2, out) == 1 && out == d && c2 == c;
				EFOR(int, b, a + 1, n){
					c2 = c; c2[a] ^= 1; c2[b] ^= 1;
					auto c3 = c2;
					ok = ok && h.decode(c2, out) == 2 && c2 == c3;
				}
			}
			if (!ok){ printf("FAIL seed=%d n=%d\n", seed, n); return 1; }
		}
	}
	puts("OK"); return 0;
}
