#include "../../tex/contents/math/sos.hpp"
#include <cassert>

// max merge: no Library Checker problem uses it
int main(){
	mt19937_64 rng(3);
	FOR(it, 0, 199){
		int n = 1 << (rng() % 9);
		vector<ll> a(n);
		for(auto &x: a) x = (ll)(rng() % 1'000'000'001);
		auto sub = a, sup = a;
		sos(sub, [](ll &x, ll y){ maximize(x, y); });
		sos(sup, [](ll &x, ll y){ maximize(x, y); }, true);
		FOR(S, 0, n - 1){
			ll m = LLONG_MIN, M = LLONG_MIN;
			FOR(T, 0, n - 1){
				if((T & S) == T) maximize(m, a[T]);
				if((T & S) == S) maximize(M, a[T]);
			}
			assert(sub[S] == m && sup[S] == M);
		}
	}
	cout << "ok\n";
}
