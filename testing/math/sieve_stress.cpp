#include "../../tex/contents/math/sieve.hpp"
int main(){
	mt19937 rng(1);
	auto check = [&](int n){
		Sieve s(n);
		vector<int> pr;
		FOR(int, x, 1, n){
			vector<int> f; int y = x;
			for (int d = 2; d * d <= y; d++) while (y % d == 0) f.push_back(d), y /= d;
			if (y > 1) f.push_back(y);
			if (f.size() == 1) pr.push_back(x);
			if (x >= 2 && s.spf[x] != f[0]){ printf("spf n=%d x=%d\n", n, x); exit(1); }
			if (s.factorize(x) != f){ printf("fact n=%d x=%d\n", n, x); exit(1); }
		}
		if (pr != s.primes){ printf("primes n=%d\n", n); exit(1); }
	};
	FOR(int, n, 1, 300) check(n);
	EFOR(int, t, 0, 200) check(rng() % 100000 + 1);
	check(1000000);
	puts("ok");
}
