#include "template.hpp"
// ./gen <seed>: same seed, same test (same machine)
mt19937_64 rng;
ll rnd(ll l, ll r){ return l + rng() % (r - l + 1); }
vector<pair<int, int>> randTree(int n){ // 1-indexed
	vector<int> p(n + 1); iota(p.begin(), p.end(), 0);
	shuffle(p.begin() + 1, p.end(), rng);
	vector<pair<int, int>> e;
	FOR(int, i, 2, n) e.push_back({p[rnd(1, i - 1)], p[i]});
	shuffle(e.begin(), e.end(), rng); return e;
}
int main(int argc, char **argv){
	rng.seed(atoll(argv[1])); int n = rnd(1, 8);
	cout << n << '\n';
	for (auto [u, v]: randTree(n)) printf("%d %d\n", u, v);
}
