#include "../../tex/contents/math/dlog.hpp"

int main(){
	// dlog exhaustive check for m in 1..150
	FOR(ll, m, 1, 150){
		EFOR(ll, a, 0, m){
			EFOR(ll, b, 0, m){
				ll brute = -1;
				ll cur = 1 % m;
				for (ll x = 0; x < 2 * m + 5; x++){
					if (cur == b){ brute = x; break; }
					cur = cur * a % m;
				}
				ll got = dlog(a, b, m);
				if (got != brute){
					cerr << "dlog fail: a=" << a << " b=" << b << " m=" << m
					     << " got=" << got << " brute=" << brute << endl;
					exit(1);
				}
			}
		}
	}

	// dsqrt small primes
	vector<ll> primes = {2, 3, 5, 7, 11, 13, 17, 97, 101, 257};
	for (ll p : primes){
		EFOR(ll, y, 0, p){
			bool exists = false;
			EFOR(ll, x, 0, p){
				if (x * x % p == y){ exists = true; break; }
			}
			ll got = dsqrt(y, p);
			if (exists){
				if (got == -1 || got * got % p != y){
					cerr << "dsqrt fail: y=" << y << " p=" << p << " got=" << got << endl;
					exit(1);
				}
			} else {
				if (got != -1){
					cerr << "dsqrt fail (should be -1): y=" << y << " p=" << p << " got=" << got << endl;
					exit(1);
				}
			}
		}
	}

	// dsqrt large primes
	mt19937_64 rng(1);
	for (ll p : {998244353LL, 1000000007LL}){
		for (int it = 0; it < 3000; it++){
			ll y = rng() % p;
			ll got = dsqrt(y, p);
			if (got != -1){
				if (got * got % p != y){
					cerr << "dsqrt big fail: y=" << y << " p=" << p << " got=" << got << endl;
					exit(1);
				}
			} else {
				if (_pw(y, (p - 1) / 2, p) == 1){
					cerr << "dsqrt big fail (Euler): y=" << y << " p=" << p << endl;
					exit(1);
				}
			}
		}
	}

	// dlog big prime
	ll m_big = 1000000007;
	ll exp_val = 123456789;
	ll b_big = _pw(5, exp_val, m_big);
	ll ans_big = dlog(5, b_big, m_big);
	if (ans_big != exp_val){
		cerr << "dlog big prime fail: got=" << ans_big << " exp=" << exp_val << endl;
		exit(1);
	}

	puts("ok");
	return 0;
}
