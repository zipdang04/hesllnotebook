#include "../../tex/contents/math/fft.hpp"
mt19937_64 rng(2024);
ll rnd(ll l, ll r){ return l + rng() % (r - l + 1); }
void fail(const char *what, int it, int n, int m){ printf("FAIL %s iter=%d n=%d m=%d\n", what, it, n, m); exit(1); }
const ll P = 1000000007;
ll md(ll x, ll p){ return (x % p + p) % p; }
ll eval(const vector<ll> &v, ll r, ll p){ ll s = 0; REV(int, i, (int)v.size() - 1, 0) s = (s * r + md(v[i], p)) % p; return s; }
// small: c == naive (exact, then mod p); big: c(r) == a(r) b(r) mod p at random r (Schwartz-Zippel)
void check(const vector<ll> &a, const vector<ll> &b, const vector<ll> &c, ll p, bool big, const char *what, int it){
	int n = a.size(), m = b.size();
	if ((int)c.size() != n + m - 1) fail(what, it, n, m);
	if (big){
		ll q = p ? p : P; // conv: exact values, compared mod P
		FOR(int, t, 0, 2){ ll r = rnd(1, q - 1); if (eval(c, r, q) != eval(a, r, q) * eval(b, r, q) % q) fail(what, it, n, m); }
		return;
	}
	FOR(int, k, 0, n + m - 2){
		ll s = 0; FOR(int, i, max(0, k - m + 1), (min(n, k + 1)) - 1) s = p ? (s + a[i] * b[k - i]) % p : s + a[i] * b[k - i];
		if (c[k] != s) fail(what, it, n, m);
	}
}
// kind: 0 uniform, 1 near M-1, 2/3 balanced worst (+/-), 4 fft_killer (digits near 2^15), -1 mixed per element
template<int M> void testMod(int n, int m, int it, int kind){
	vector<ll> a(n), b(m);
	ll c = sqrt(M), h = c / 2, B = 1 << 15;
	for (auto *v: {&a, &b}) for (ll &x: *v){
		int t = kind >= 0 ? kind : rng() % 5;
		if (t == 0) x = rnd(0, M - 1);
		if (t == 1) x = M - 1 - rnd(0, min(1000, M - 1));
		if (t >= 2) x = ((h - rnd(0, min(h, 50ll))) * c + h - rnd(0, min(h, 50ll))) % M;
		if (t == 3) x = (M - x) % M;
		if (t == 4) x = M > B * B ? rnd(M / B - 1000, M / B - 1) * B + rnd(B - 1000, B - 1) : rnd(0, M - 1);
	}
	check(a, b, convMod<M>(a, b), M, kind >= 0, "convMod", it);
}
void testReal(int n, int m, ll V, bool neg, int it, bool big){
	vector<ll> a(n), b(m);
	for (auto *v: {&a, &b}) for (ll &x: *v) x = rng() % 2 ? rnd(neg ? -V : 0, V) : V - rnd(0, V / 100);
	vector<double> r = conv(vector<double>(a.begin(), a.end()), vector<double>(b.begin(), b.end()));
	vector<ll> c; for (double x: r) c.push_back(llround(x));
	check(a, b, c, 0, big, "conv", it);
}
int main(){
	FOR(int, it, 1, 2000){ // small: full naive
		int n = rnd(1, it < 100 ? 4 : 300), m = rnd(1, it < 100 ? 4 : 300);
		testReal(n, m, rnd(1, 1e5), rng() % 2, it, 0);
		testMod<1000000007>(n, m, it, -1); testMod<998244353>(n, m, it, -1); testMod<7>(n, m, it, -1);
	}
	if (conv({}, {1.0}).size() || convMod<7>({}, {1}).size()) fail("empty", 0, 0, 1);
	// limits: conv (sum a^2 + sum b^2) log2 n ~ 9e14 at n = 2^17; convMod at |a| = |b| = 2^19
	FOR(int, it, 1, 3) testReal(65536, 65536, 20000, 0, it, 1);
	FOR(int, kind, 0, 4){ testMod<1000000007>(1 << 19, 1 << 19, kind, kind); testMod<998244353>(1 << 19, 1 << 19, kind, kind); }
	puts("OK");
}
