#include "../../tex/contents/math/linearrec.hpp"
#include <cassert>

int main(){
	mt19937 rng(0);
	FOR(it, 0, 7){
		int d = rng() % 41;
		if(it == 0) d = 0;
		if(it == 1) d = 31;
		if(it == 2) d = 32;
		if(it == 3) d = 40;
		vector<MintN> c(d), s(d);
		for(auto &x: c) x = rng() % 998244353;
		if(d) c[d-1] = rng() % 998244352 + 1;
		for(auto &x: s) x = rng() % 998244353;
		vector<int> ks(5);
		for(int &k: ks) k = rng() % 1000001;
		int limit = max(3 * d + 49,
			*max_element(ks.begin(), ks.end()));
		FOR(i, d, limit){
			MintN x = 0;
			FOR(j, 0, d - 1) x += c[j] * s[i-1-j];
			s.push_back(x);
		}
		vector<MintN> prefix(s.begin(),
			s.begin() + 2 * d + 5);
		vector<MintN> b = berlekampMassey(prefix);
		int n = sz(b);
		assert(n <= d);
		FOR(i, n, 3 * d + 49){
			MintN x = 0;
			FOR(j, 0, n - 1) x += b[j] * s[i-1-j];
			assert(x == s[i]);
		}
		FOR(k, 0, 3 * d + 49){
			assert(kthTerm(s, c, k) == s[k]);
			assert(kthTerm(s, b, k) == s[k]);
		}
		for(int k: ks){
			assert(kthTerm(s, c, k) == s[k]);
			assert(kthTerm(s, b, k) == s[k]);
		}
	}
}
