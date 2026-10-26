#include "../../tex/contents/math/primes.hpp"
int main(){
	int q; read(q);
	FOR(int, _, 1, q){
		ull n; read(n);
		auto v = factor(n);
		sort(v.begin(), v.end());
		printf("%d", (int)v.size());
		for (ull p: v) printf(" %llu", p);
		puts("");
	}
}
