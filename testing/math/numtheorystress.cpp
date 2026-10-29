#include "../../tex/contents/math/crt.hpp"
#include "../../tex/contents/math/floorsum.hpp"
typedef __int128 L;
mt19937_64 rng(1);
ll rnd(ll l, ll r){ return uniform_int_distribution<ll>(l, r)(rng); }
L fdiv(L a, L b){ return a / b - (a % b < 0); }
int main(){
	// extGcd: AOJ criterion brute
	FOR(int, a, 1, 150) FOR(int, b, 1, 150){
		ll x, y, g = extGcd(a, b, x, y);
		if (g != __gcd(a, b) || a * x + b * y != g) { puts("eg bad"); return 1; }
		// best: min |x|+|y|, then x<=y
		ll bx = 1e9, by = 1e9;
		FOR(int, X, -200, 200) if ((g - (ll)a * X) % b == 0){
			ll Y = (g - (ll)a * X) / b;
			if (make_pair(llabs(X)+llabs(Y), X > Y) < make_pair(llabs(bx)+llabs(by), bx > by)) bx = X, by = Y;
		}
		if (bx != x || by != y){ printf("eg aoj %d %d: %lld %lld vs %lld %lld\n", a, b, x, y, bx, by); return 1; }
	}
	// extGcd large/negative
	FOR(int, it, 0, 199999){
		ll a = rnd(-4e18, 4e18), b = rnd(-4e18, 4e18), x, y; if (it % 3 == 0) b = rnd(-100, 100);
		ll g = extGcd(a, b, x, y);
		if ((L)a * x + (L)b * y != g || llabs(g) != std::gcd(a, b)) { printf("eg2 %lld %lld\n", a, b); return 1; }
	}
	// crt small brute
	FOR(int, m, 1, 40) FOR(int, n, 1, 40) FOR(int, a, -m, 2*m) FOR(int, b, -n, 2*n){
		auto [x, l] = crt(a, m, b, n); ll L0 = lcm(m, n), ex = -1;
		FOR(int, z, 0, L0 - 1) if (((z - a) % m + m) % m == 0 && ((z - b) % n + n) % n == 0){ ex = z; break; }
		if (ex != x || (ex >= 0 && l != L0) || (ex < 0 && l != -1)){ printf("crt %d %d %d %d\n", a, m, b, n); return 1; }
	}
	// crt big: lcm up to 9.2e18, m, n up to 4e18, any a, b
	FOR(int, it, 0, 199999){
		ll g = rnd(1, it % 2 ? 1 : 1000000), m = g * rnd(1, 3e9), n = g * rnd(1, 3e9);
		if (it % 8 == 2) g = rnd(1, 2e18), m = g * rnd(1, 2), n = g * rnd(1, 2); // m, n up to 4e18
		ll a = rnd(-9e18, 9e18), b = rnd(-9e18, 9e18);
		if (it % 4 == 0) b = a + g * rnd(-1e6, 1e6);
		g = std::gcd(m, n); if ((L)m / g * n > LLONG_MAX) continue;
		auto [x, l] = crt(a, m, b, n);
		bool ok = ((L)a - b) % g == 0;
		if (!ok){ if (x != -1 || l != -1){ puts("crt big none"); return 1; } continue; }
		if (l != m / g * n || x < 0 || x >= l || ((L)x - a) % m || ((L)x - b) % n){ printf("crt big %lld %lld %lld %lld -> %lld %lld\n", a, m, b, n, x, l); return 1; }
	}
	// floorSum small brute incl negatives and large |a|,|b|
	FOR(int, it, 0, 299999){
		ll n = rnd(0, 60), m = rnd(1, it % 2 ? 50 : (1ll << 32) - 1);
		ll lim = it % 3 == 0 ? (ll)4e18 / max(1ll, n * n) : 100;
		ll a = rnd(-lim, lim), b = rnd(-lim, lim);
		L s = 0; FOR(int, i, 0, n - 1) s += fdiv((L)a * i + b, m);
		if (s > LLONG_MAX || s < LLONG_MIN) continue;
		if (floorSum(n, m, a, b) != (ll)s){ printf("fs %lld %lld %lld %lld\n", n, m, a, b); return 1; }
	}
	// floorSum near limits: n = m = 2^32 - 1, closed forms
	{
		ll M = (1ll << 32) - 1, n = M;
		L e = (L)n * (n + 1) / 2 - n; // floor((M-1)(i+1)/M) = i for i < M
		if (floorSum(n, M, M - 1, M - 1) != (ll)e){ puts("fs lim1"); return 1; }
		if (floorSum(n, M, M, 0) != (ll)((L)n * (n - 1) / 2)){ puts("fs lim2"); return 1; } // a = M: sum i
		// floor(-(M-1)j/M) = -j for 1 <= j < M, -(M-1) for j = M
		if (floorSum(n, M, -(M - 1), -(M - 1)) != (ll)(-(L)n*(n+1)/2 + 1)){ puts("fs lim3"); return 1; }
	}
	// floorSum medium brute n up to 1e5, m up to 2^32-1
	FOR(int, it, 0, 299){
		ll n = rnd(0, 100000), m = rnd(1, (1ll << 32) - 1), a = rnd(-4e18, 4e18) / n / n, b = rnd(-4e18, 4e18) / n;
		if (n == 0) a = b = 0;
		L s = 0; FOR(int, i, 0, n - 1) s += fdiv((L)a * i + b, m);
		if (s > LLONG_MAX || s < LLONG_MIN) continue;
		if (floorSum(n, m, a, b) != (ll)s){ printf("fs med %lld %lld %lld %lld\n", n, m, a, b); return 1; }
	}
	puts("OK");
}
