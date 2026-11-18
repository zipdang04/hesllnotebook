#include "../../tex/contents/math/lagrange.hpp"
#include <cassert>
#include "../../tex/contents/math/gauss.hpp"

MintN eval(const vector<MintN> &c, MintN x){
	MintN res = 0;
	for(int i = sz(c) - 1; i >= 0; i--)
		res = res * x + c[i];
	return res;
}

MintN brute(const vector<MintN> &y, MintN t){
	int n = sz(y);
	MintN res = 0;
	FOR(i, 0, n - 1){
		MintN term = y[i];
		FOR(j, 0, n - 1) if(i != j)
			term *= (t - MintN(j)) / MintN(i - j);
		res += term;
	}
	return res;
}

int main(){
	mt19937_64 rng(0);
	FOR(it, 0, 2999){
		int n = rng() % 30 + 1;
		vector<MintN> x, y(n), nodes(n);
		while(sz(x) < n){
			MintN v = rng() % (it % 2 ? 998244353 : 40);
			if(find(x.begin(), x.end(), v) == x.end())
				x.push_back(v);
		}
		FOR(i, 0, n - 1){
			y[i] = rng() % 998244353;
			nodes[i] = i;
		}
		vector<MintN> c = interpolate(x, y);
		assert(sz(c) == n);
		FOR(i, 0, n - 1) assert(eval(c, x[i]) == y[i]);
		if(it < 300){
			vector<vector<MintN>> v(n, vector<MintN>(n));
			FOR(i, 0, n - 1){
				v[i][0] = 1;
				FOR(j, 1, n - 1)
					v[i][j] = v[i][j - 1] * x[i];
			}
			auto s = solveLinear(v, y);
			assert(sz(s) == 1);
			assert(s[0] == c);
		}
		vector<MintN> c0 = interpolate(nodes, y);
		vector<ll> ts = {0, n - 1, n, (ll)(rng() % 1000),
			998244353LL + 5, 1000000000000000000LL, -3};
		for(ll t: ts){
			MintN ans = interpolateAt(y, t);
			assert(ans == eval(c0, MintN(t)));
			assert(ans == brute(y, MintN(t)));
		}
	}
}
