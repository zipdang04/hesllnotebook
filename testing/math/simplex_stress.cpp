#include "../../tex/contents/math/simplex.hpp"
#include <cassert>

ll det(const vector<vector<ll>> &a){
	int n = (int)a.size();
	vector<int> p(n);
	iota(p.begin(), p.end(), 0);
	ll res = 0;
	do{
		ll term = 1;
		int inv = 0;
		FOR(i, 0, n - 1){
			term *= a[i][p[i]];
			FOR(j, 0, i - 1) if(p[j] > p[i]) inv++;
		}
		res += inv % 2 ? -term : term;
	}while(next_permutation(p.begin(), p.end()));
	return res;
}

pair<bool, double> brute(vector<vector<ll>> a,
		vector<ll> b, const vector<ll> &c){
	int n = (int)c.size();
	FOR(j, 0, n - 1){
		vector<ll> row(n);
		row[j] = -1;
		a.push_back(row);
		b.push_back(0);
	}
	int m = (int)b.size();
	bool found = false;
	ll best = 0, bestDen = 1;
	FOR(mask, 0, (1 << m) - 1){
		if(__builtin_popcount((unsigned)mask) != n) continue;
		vector<vector<ll>> mat;
		vector<ll> rhs;
		FOR(i, 0, m - 1) if(mask >> i & 1){
			mat.push_back(a[i]);
			rhs.push_back(b[i]);
		}
		ll den = det(mat);
		if(!den) continue;
		vector<ll> num(n);
		FOR(j, 0, n - 1){
			auto tmp = mat;
			FOR(i, 0, n - 1) tmp[i][j] = rhs[i];
			num[j] = det(tmp);
		}
		if(den < 0){
			den = -den;
			for(ll &v: num) v = -v;
		}
		bool ok = true;
		FOR(i, 0, m - 1){
			ll sum = 0;
			FOR(j, 0, n - 1) sum += a[i][j] * num[j];
			if(sum > b[i] * den) ok = false;
		}
		if(!ok) continue;
		ll val = 0;
		FOR(j, 0, n - 1) val += c[j] * num[j];
		if(!found || val * bestDen > best * den){
			found = true;
			best = val;
			bestDen = den;
		}
	}
	return {found, (double)best / bestDen};
}

int main(){
	mt19937 rng(1234567);
	auto rnd = [&](int lo, int hi){
		return uniform_int_distribution<int>(lo, hi)(rng);
	};
	int bounded = 0, unbounded = 0;
	while(bounded < 1600 || unbounded < 400){
		bool cap = bounded < 1600;
		int n = rnd(1, 4), m = rnd(0, 4);
		vector<vector<ll>> a(m, vector<ll>(n));
		vector<ll> b(m), c(n);
		FOR(i, 0, m - 1){
			FOR(j, 0, n - 1) a[i][j] = rnd(-5, 5);
			b[i] = rnd(0, 4) < 2 ? 0 : rnd(-5, 5);
		}
		if(cap) FOR(j, 0, n - 1){
			vector<ll> row(n);
			row[j] = 1;
			a.push_back(row);
			b.push_back(10);
		}
		FOR(j, 0, n - 1) c[j] = rnd(-5, 5);
		auto [found, best] = brute(a, b, c);
		if(!cap && !found) continue;
		if(cap) bounded++;
		else unbounded++;
		vector<vector<double>> A(a.size(), vector<double>(n));
		vector<double> B(b.begin(), b.end());
		vector<double> C(c.begin(), c.end());
		FOR(i, 0, (int)a.size() - 1)
			FOR(j, 0, n - 1) A[i][j] = a[i][j];
		Simplex lp(A, B, C);
		if(!found){
			assert(lp.val == -oo && lp.x.empty());
			continue;
		}
		assert(lp.val != -oo);
		if(cap) assert(abs(lp.val - best) <= 1e-6);
		if(lp.val == oo) continue;
		assert((int)lp.x.size() == n);
		double val = 0;
		FOR(j, 0, n - 1){
			assert(lp.x[j] >= -1e-6);
			val += c[j] * lp.x[j];
		}
		FOR(i, 0, (int)a.size() - 1){
			double sum = 0;
			FOR(j, 0, n - 1) sum += a[i][j] * lp.x[j];
			assert(sum <= b[i] + 1e-6);
		}
		assert(abs(val - lp.val) <= 1e-6);
	}
	return 0;
}
