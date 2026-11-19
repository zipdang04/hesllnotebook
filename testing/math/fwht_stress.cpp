#include "../../tex/contents/math/fwht.hpp"
#include <cassert>

typedef Mint<7> M7;
int apply(int i, int j, char op){
	return op == '^' ? i ^ j : op == '&' ? i & j : i | j;
}
template<int MOD>
vector<Mint<MOD>> brute(const vector<Mint<MOD>> &a,
		const vector<Mint<MOD>> &b, char op){
	int n = 1;
	while(n < max(sz(a), sz(b))) n *= 2;
	vector<Mint<MOD>> c(n);
	FOR(i, 0, sz(a) - 1) FOR(j, 0, sz(b) - 1)
		c[apply(i, j, op)] += a[i] * b[j];
	return c;
}
template<int MOD>
vector<Mint<MOD>> randVec(int n, mt19937_64 &rng){
	vector<Mint<MOD>> a(n);
	for(auto &x: a) x = rng() % MOD;
	return a;
}

int main(){
	mt19937_64 rng(2);
	const string ops = "^&|";
	FOR(it, 0, 299){
		int n = rng() % (it < 200 ? 40 : 1024) + 1;
		int m = rng() % (it < 200 ? 40 : 1024) + 1;
		for(char op: ops){
			auto a = randVec<998244353>(n, rng);
			auto b = randVec<998244353>(m, rng);
			assert(convBit(a, b, op) == brute(a, b, op));
			auto x = randVec<7>(n, rng), y = randVec<7>(m, rng);
			assert(convBit(x, y, op) == brute(x, y, op));
		}
	}
	FOR(k, 0, 10) for(char op: ops){
		int n = 1 << k;
		auto a = randVec<998244353>(n, rng), f = a, back = a;
		fwht(f, op);
		// forward '|': subset sums; forward '&': superset sums
		FOR(s, 0, n - 1) if(op != '^'){
			MintN sum = 0;
			FOR(t, 0, n - 1){
				int need = op == '|' ? (t & s) == t : (t & s) == s;
				if(need) sum += a[t];
			}
			assert(f[s] == sum);
		}
		fwht(back, op), fwht(back, op, true);
		assert(back == a);
	}
	// text recipe: max convolution by prefix sums, min by
	// suffix sums
	FOR(it, 0, 199){
		int n = rng() % 50 + 1;
		auto a = randVec<998244353>(n, rng);
		auto b = randVec<998244353>(n, rng);
		vector<MintN> mx(n), mn(n), pa(n), pb(n), sa(n), sb(n);
		FOR(i, 0, n - 1) FOR(j, 0, n - 1){
			mx[max(i, j)] += a[i] * b[j];
			mn[min(i, j)] += a[i] * b[j];
		}
		FOR(k, 0, n - 1){
			pa[k] = a[k] + (k ? pa[k - 1] : 0);
			pb[k] = b[k] + (k ? pb[k - 1] : 0);
		}
		REV(k, n - 1, 0){
			sa[k] = a[k] + (k < n - 1 ? sa[k + 1] : 0);
			sb[k] = b[k] + (k < n - 1 ? sb[k + 1] : 0);
		}
		FOR(k, 0, n - 1){
			MintN pc = pa[k] * pb[k], sc = sa[k] * sb[k];
			if(k) pc -= pa[k - 1] * pb[k - 1];
			if(k < n - 1) sc -= sa[k + 1] * sb[k + 1];
			assert(pc == mx[k] && sc == mn[k]);
		}
	}
}
