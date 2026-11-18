#include "../../tex/contents/flow/hungarian.hpp"
#include <cassert>

int main(){
	mt19937_64 rng(712367);
	FOR(it, 1, 2000){
		int n = rng() % 6;
		int lo = max(n, 1);
		int m = lo + rng() % (8 - lo);
		vector<vector<ll>> a(n + 1,
			vector<ll>(m + 1, -1000000));
		FOR(i, 1, n) FOR(j, 1, m)
			a[i][j] = (int)(rng() % 21) - 10;
		vector<int> p(m);
		iota(all(p), 1);
		ll best = oo;
		do{
			ll sum = 0;
			FOR(i, 1, n) sum += a[i][p[i-1]];
			best = min(best, sum);
		}while(next_permutation(all(p)));
		auto [cost, match] = hungarian(a);
		assert(cost == best);
		assert(sz(match) == n + 1);
		assert(match[0] == 0);
		vector<char> seen(m + 1);
		ll sum = 0;
		FOR(i, 1, n){
			int j = match[i];
			assert(1 <= j && j <= m);
			assert(!seen[j]);
			seen[j] = 1;
			sum += a[i][j];
		}
		assert(sum == cost);
	}
	return 0;
}
