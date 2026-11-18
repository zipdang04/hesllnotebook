#include "../../tex/contents/math/crt.hpp"
#include <cassert>

pair<ll, ll> brute(const vector<pair<ll, ll>> &eq){
	ll l = 1;
	for(auto [a, m]: eq) l = l / __gcd(l, m) * m;
	FOR(r, 0LL, l - 1){
		bool ok = true;
		for(auto [a, m]: eq)
			if((r - a) % m) ok = false;
		if(ok) return {r, l};
	}
	return {-1, -1};
}

int main(){
	FOR(a, 0LL, 60)
		FOR(b, 0LL, 60){
			ll x, y;
			ll g = extGcd(a, b, x, y);
			assert(g == __gcd(a, b));
			assert(a * x + b * y == g);
			if(!b) assert(x == 1 && y == 0);
			if(a && b){
				assert(abs(x) <= b / g);
				assert(abs(y) <= a / g);
			}
		}
	FOR(m, 1LL, 40)
		FOR(n, 1LL, 40)
			FOR(a, 0LL, m - 1)
				FOR(b, 0LL, n - 1){
					auto want = brute({{a, m}, {b, n}});
					assert(crt(a, m, b, n) == want);
					if(a > 0)
						assert(crt(a - m, m, b, n) == want);
				}
	FOR(m, 1LL, 12)
		FOR(n, 1LL, 12)
			FOR(p, 1LL, 12)
				FOR(t, 0LL, 31){
					ll a = t % m;
					ll b = (7 * t + t / 3) % n;
					ll c = (11 * t + t / 5) % p;
					vector<pll> eq = {
						{a, m}, {b, n}, {c, p}
					};
					pll got = {0, 1};
					for(auto [r, mod]: eq){
						got = crt(got.fi, got.se, r, mod);
						if(got.fi == -1) break;
					}
					assert(got == brute(eq));
				}
	return 0;
}
