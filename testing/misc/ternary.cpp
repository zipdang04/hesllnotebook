#include "../../tex/contents/misc/ternary.hpp"

int main(){
	mt19937_64 rg(5);
	auto R = [&](ll a, ll b){ return (ll)(rg() % (b - a + 1)) + a; };

	// Check 1: 200000 seeds integer ternary search
	for (int sd = 0; sd < 200000; sd++){
		ll l = R(-30, 30), r = l + R(0, 40);
		ll p = R(l, r);
		ll plat = R(0, 1) ? R(0, 4) : 0;
		ll k = R(1, 5);
		auto f = [&](ll x) -> ll {
			if (x < p) return (p - x) * k * 3 + (p - x) * (p - x);
			if (x <= p + plat) return 0;
			return (x - p - plat) * k + (x - p - plat) * (x - p - plat);
		};
		ll b = l;
		for (ll x = l; x <= r; x++) if (f(x) < f(b)) b = x;
		if (ternaryInt(l, r, f) != b){
			printf("CHECK 1 FAIL: seed %d, l=%lld, r=%lld, got %lld, expected %lld\n", sd, l, r, ternaryInt(l, r, f), b);
			return 1;
		}
	}

	// Check 2: ternaryInt(5,5,f) == 5 for f that asserts if called; ternaryInt(3,4,..) both orders
	{
		auto f_assert = [](ll) -> ll { exit(2); };
		if (ternaryInt(5, 5, f_assert) != 5){
			puts("CHECK 2 FAIL: ternaryInt(5,5,..) did not return 5");
			return 1;
		}
		auto f1 = [](ll x) -> ll { return x == 3 ? 0 : 10; };
		if (ternaryInt(3, 4, f1) != 3){
			puts("CHECK 2 FAIL: ternaryInt(3,4,..) min at 3 did not return 3");
			return 1;
		}
		auto f2 = [](ll x) -> ll { return x == 4 ? 0 : 10; };
		if (ternaryInt(3, 4, f2) != 4){
			puts("CHECK 2 FAIL: ternaryInt(3,4,..) min at 4 did not return 4");
			return 1;
		}
		auto f3 = [](ll) -> ll { return 0; };
		if (ternaryInt(3, 4, f3) != 3){
			puts("CHECK 2 FAIL: ternaryInt(3,4,..) flat minimum did not return smallest argmin 3");
			return 1;
		}
	}

	// Check 3: f(x) = llabs(x - 3e18) on [-4e18, 4e18] == 3e18
	{
		auto f = [](ll x) -> ll { return llabs(x - (ll)3e18); };
		if (ternaryInt(-(ll)4e18, (ll)4e18, f) != (ll)3e18){
			puts("CHECK 3 FAIL: near ll limit 4e18 failed");
			return 1;
		}
	}

	// Check 4: 100000 seeds goldenSection real search
	double worst = 0;
	for (int sd = 0; sd < 100000; sd++){
		double a = ((ll)(rg() % 2000) - 1000) / 10.0;
		double w = (sd % 3 == 0) ? 1e-9 : (sd % 3 == 1 ? (rg() % 1000) / 10.0 : 1e3);
		double b = a + w;
		double c = a + (b - a) * (rg() % 10001) / 10000.0;
		double k = 1 + rg() % 5;
		auto f = [&](double x){ return sd % 2 ? fabs(x - c) * k : (x - c) * (x - c) * k + 3; };
		double x = goldenSection(a, b, f);
		double tol = sd % 2 ? 1e-9 * (1 + fabs(c)) : 1e-6 * (1 + w);
		worst = max(worst, fabs(x - c));
		if (fabs(x - c) > tol || x < a || x > b){
			printf("CHECK 4 FAIL %d x=%.12f c=%.12f w=%g\n", sd, x, c, w);
			return 1;
		}
		double grid_min = f(c);
		if (sd % 200 == 0){
			for (int i = 0; i <= 10000; i++){
				double gx = a + (b - a) * i / 10000.0;
				grid_min = min(grid_min, f(gx));
			}
		}
		if (f(x) > grid_min + 1e-9 * (1 + fabs(f(x)))){
			printf("CHECK 4 GRID MIN FAIL %d f(x)=%.12f grid_min=%.12f\n", sd, f(x), grid_min);
			return 1;
		}
	}

	// Check 5: goldenSection(2,2,f) == 2
	{
		auto f = [](double x){ return (x - 2) * (x - 2); };
		double x = goldenSection(2.0, 2.0, f);
		if (fabs(x - 2.0) > 1e-15){
			puts("CHECK 5 FAIL: a == b did not return 2");
			return 1;
		}
	}

	// Check 6: anneal on convex 2D smoke
	{
		using P = array<double, 2>;
		auto en = [](const P &p){ return (p[0] - 3) * (p[0] - 3) + (p[1] + 2) * (p[1] + 2); };
		auto nb = [](const P &p, double T, mt19937_64 &g){
			normal_distribution<double> nd(0, 1);
			double st = max(1e-6, sqrt(T));
			return P{p[0] + nd(g) * st, p[1] + nd(g) * st};
		};
		P b = anneal(P{0, 0}, en, nb, 0.2, 10, 1e-9);
		if (en(b) >= 1e-6){
			printf("CHECK 6 FAIL: convex energy %g >= 1e-6\n", en(b));
			return 1;
		}
	}

	// Check 7: TSP n=8, 20 instances
	{
		for (int sd = 0; sd < 20; sd++){
			int n = 8;
			vector<double> X(n), Y(n);
			for (int i = 0; i < n; i++) X[i] = rg() % 1000, Y[i] = rg() % 1000;
			auto len = [&](const vector<int> &p){
				double s = 0;
				for (int i = 0; i < n; i++){
					int u = p[i], v = p[(i + 1) % n];
					s += hypot(X[u] - X[v], Y[u] - Y[v]);
				}
				return s;
			};
			vector<int> p(n);
			iota(p.begin(), p.end(), 0);
			double opt = 1e18;
			do {
				opt = min(opt, len(p));
			} while (next_permutation(p.begin() + 1, p.end()));
			iota(p.begin(), p.end(), 0);
			auto nb = [&](const vector<int> &q, double, mt19937_64 &g){
				auto r = q;
				int i = g() % n, j = g() % n;
				if (i > j) swap(i, j);
				reverse(r.begin() + i, r.begin() + j + 1);
				return r;
			};
			auto b = anneal(p, len, nb, 0.1, 100, 1e-3);
			if (len(b) > opt + 1e-9){
				printf("CHECK 7 FAIL: instance %d got %g, opt %g\n", sd, len(b), opt);
				return 1;
			}
		}
	}

	// Check 8: plateau away from minimum
	{
		vector<ll> vals = {5, 5, 5, 5, 1};
		auto f = [&](ll x) -> ll { return vals[x]; };
		ll res = ternaryInt((ll)0, (ll)4, f);
		if (res != 0){
			printf("CHECK 8 FAIL: plateau away from min got %lld expected 0\n", res);
			return 1;
		}
	}

	// Check 9: anneal with seconds=0 returns initial state unchanged
	{
		int start = 42;
		auto en = [](int x) -> double { return x * x; };
		auto nb = [](int x, double, mt19937_64 &) -> int { return x + 1; };
		int res = anneal(start, en, nb, 0.0);
		if (res != start){
			printf("CHECK 9 FAIL: seconds=0 got %d expected %d\n", res, start);
			return 1;
		}
	}

	return 0;
}
