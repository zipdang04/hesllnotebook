#include "../../tex/contents/ds/lazyseg.hpp"
#include <cassert>

int main(){
	mt19937_64 rng(123456789);
	const ll mod = 998244353;
	FOR(n, 1, 33)
		FOR(round, 1, 100){
			vector<LazySeg::Node> a(n);
			vector<ll> brute(n);
			FOR(i, 0, n - 1){
				brute[i] = rng() % mod;
				a[i] = {brute[i], 1};
			}
			LazySeg seg(a);
			FOR(step, 1, 1000){
				int l = rng() % n, r = rng() % n;
				if(l > r)
					swap(l, r);
				if(rng() % 2){
					ll b = rng() % mod, c = rng() % mod;
					if(rng() % 2)
						b = rng() % 3;
					LazySeg::Tag t{b, c};
					if(rng() % 10 == 0){
						t = LazySeg::Tag();
						b = 1;
						c = 0;
					}
					seg.update(l, r, t);
					FOR(i, l, r)
						brute[i] = (b * brute[i] + c) % mod;
				}else{
					ll sum = 0;
					FOR(i, l, r)
						sum = (sum + brute[i]) % mod;
					auto got = seg.get(l, r);
					assert(got.sum == sum);
					assert(got.len == r - l + 1);
				}
			}
		}
	cout << "OK" << '\n';
}
