// ./gen seed > in: same seed, same test. Small n first.
#include <bits/stdc++.h>
using namespace std;
mt19937_64 rng;
long long rnd(long long l, long long r){
	return rng() % (r - l + 1) + l;
}
int main(int, char **argv){
	rng.seed(atoll(argv[1]));
	int n = rnd(1, 8);
	printf("%d\n", n);
	vector<int> p(n);
	iota(p.begin(), p.end(), 1);
	shuffle(p.begin(), p.end(), rng);
	// random tree: random earlier parent, random labels
	for(int i = 1; i < n; i++){
		int u = p[rnd(0, i - 1)], v = p[i];
		if(rnd(0, 1)) swap(u, v);
		printf("%d %d\n", u, v);
	}
}
