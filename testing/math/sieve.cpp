#include "../../tex/contents/math/sieve.hpp"
int main(){
	int n, a, b;
	scanf("%d %d %d", &n, &a, &b);
	Sieve s(n);
	int pi = s.primes.size();
	printf("%d", pi);
	int c = 0;
	for (int i = b; i < pi; i += a) c++;
	printf(" %d\n", c);
	bool first = true;
	for (int i = b; i < pi; i += a){
		printf(first ? "%d" : " %d", s.primes[i]);
		first = false;
	}
	puts("");
}
