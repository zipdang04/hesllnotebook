#include "../../tex/contents/contest/fastio.hpp"
static_assert(STRESS == 0, "default STRESS must be 0");
int main(){
	dbg(1, 2); // must vanish without -DLOCAL
	const int B = STRESS ? 2 : 300; assert(B == 300);
	const char *f = "/tmp/hesl_fastio_stress.txt";
	mt19937_64 rng(12345);
	const char *ws[] = {" ", "\n", "\r\n", "\t", "   ", "\n\n"};
	for (int seed = 0; seed < 300; seed++){
		int n = seed < 5 ? seed : rng() % 40000; // 40000 numbers ~ >64 KB: crosses buffer refills
		vector<ll> v(n);
		string s = seed % 3 ? "" : "\n ";
		for (ll &x: v){
			int k = rng() % 6;
			if (k == 0) x = 0;
			else if (k == 1) x = (ll)(rng() % 2 ? 1 : -1) * (ll)(9e18 - rng() % 1000);
			else if (k == 2) x = (ll)(rng() % 2 ? 1 : -1) * (ll)(rng() % 3000000000LL);
			else x = (ll)(rng() % 2001) - 1000;
			s += to_string(x) + ws[rng() % 6];
		}
		FILE *o = fopen(f, "w"); fputs(s.c_str(), o); fclose(o);
		assert(freopen(f, "r", stdin));
		for (int i = 0; i < n; i++){
			ll y; bool ok = readInt(y);
			if (!ok || y != v[i]){ printf("seed %d idx %d: got %lld want %lld\n", seed, i, y, v[i]); return 1; }
		}
		ll y; int z;
		if (readInt(y) || readInt(z) || readInt(y, z)){ printf("seed %d: readInt true at EOF\n", seed); return 1; }
	}
	// int type, variadic with several types, partial failure
	{ FILE *o = fopen(f, "w"); fputs("-2147483647 5 7 x", o); fclose(o); freopen(f, "r", stdin);
	  int a; ll b; unsigned c; assert(readInt(a, b, c) && a == -2147483647 && b == 5 && c == 7u); assert(_gc() == 'x' && _gc() == 0 && _gc() == 0); }
	remove(f);
}
