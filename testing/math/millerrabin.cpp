#include "../../tex/contents/math/primes.hpp"
int main(){
	int q; read(q);
	FOR(int, _, 1, q){
		ull n; read(n);
		puts(isPrime(n) ? "Yes" : "No");
	}
}
