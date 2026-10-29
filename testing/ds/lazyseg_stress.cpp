#include "../../tex/contents/ds/lazyseg.hpp"
const ll P = 1e9 + 7, B = 131;
struct Hash{ // S = (h, B^len, 1 + B + ... + B^(len-1)); F = assign c (-1: none). op/cp not commutative
	using S = array<ll, 3>; using F = ll;
	static S op(S x, S y){ return {(x[0] * y[1] + y[0]) % P, x[1] * y[1] % P, (x[2] * y[1] + y[2]) % P}; }
	static S e(){ return {0, 1, 0}; }
	static S mp(F f, S x){ return f < 0 ? x : S{f * x[2] % P, x[1], x[2]}; }
	static F cp(F f, F g){ return f < 0 ? g : f; }
	static F id(){ return -1; }
};
mt19937 rng(1);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(rng); }
void fail(const char* w, int n){ printf("FAIL %s n=%d\n", w, n); exit(1); }
int main(){
	FOR(int, it, 0, 2999){
		int n = it < 2000 ? rnd(1, 8) : rnd(1, 60);
		vector<ll> a(n + 1), h(n + 1); // brute, 1-indexed
		FOR(int, i, 1, n) a[i] = rnd(-1e9, 1e9), h[i] = rnd(0, 5);
		LazySeg<_exampleAddMin> s1(vector<ll>(a.begin() + 1, a.end()));
		vector<Hash::S> hv; FOR(int, i, 1, n) hv.push_back({h[i], B, 1});
		LazySeg<Hash> s2(hv);
		FOR(int, qq, 0, 299){
			int L = rnd(1, n), R = rnd(1, n); if (L > R) swap(L, R);
			int op = rnd(0, 5);
			if (op == 0){ ll f = rnd(-1e9, 1e9); s1.apply(L, R, f); FOR(int, i, L, R) a[i] += f; }
			else if (op == 1){ ll c = rnd(0, 5); s2.apply(L, R, c); FOR(int, i, L, R) h[i] = c; }
			else if (op == 2){
				if (rnd(0, 3) == 0) L = rnd(1, n + 1), R = L - 1; // empty range
				ll m = LLONG_MAX; FOR(int, i, L, R) m = min(m, a[i]);
				if (s1.query(L, R) != m) fail("min", n);
			} else if (op == 3){
				if (rnd(0, 3) == 0) L = rnd(1, n + 1), R = L - 1;
				Hash::S x{0, 1, 0}; FOR(int, i, L, R) x = {(x[0] * B + h[i]) % P, x[1] * B % P, (x[2] * B + 1) % P};
				if (s2.query(L, R) != x) fail("hash", n);
			} else {
				L = rnd(1, n + 1); ll K = rnd(0, 2) ? a[rnd(1, n)] : rnd(-3e9, 3e9);
				int r = L - 1; while (r < n && a[r + 1] >= K) r++;
				if (s1.maxRight(L, [&](ll v){ return v >= K; }) != r) fail("maxRight", n);
			}
		}
	}
	puts("OK");
}
