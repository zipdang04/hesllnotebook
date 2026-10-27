#pragma once
#include "../contest/template.hpp"

// min of lines / segments y = ax + b at query points xs (fixed up front)
struct LiChao{
	struct Line{ll a, b; ll operator()(ll x) const{ return a * x + b; }};
	int n; vector<ll> xs; vector<Line> t;
	LiChao(vector<ll> x): xs(x){
		sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
		n = xs.size(); t.assign(4 * n, {0, LLONG_MAX});
	}
	int _id(ll x){ return lower_bound(xs.begin(), xs.end(), x) - xs.begin(); }
	void _ins(int k, int l, int r, Line f){ // node k covers xs[l..r)
		int m = (l + r) / 2;
		bool lef = f(xs[l]) < t[k](xs[l]), mid = f(xs[m]) < t[k](xs[m]);
		if (mid) swap(t[k], f);
		if (r - l == 1) return;
		lef != mid ? _ins(2 * k, l, m, f) : _ins(2 * k + 1, m, r, f);
	}
	void _add(int k, int l, int r, int L, int R, Line f){
		if (R <= l || r <= L) return;
		if (L <= l && r <= R) return _ins(k, l, r, f);
		int m = (l + r) / 2; _add(2 * k, l, m, L, R, f); _add(2 * k + 1, m, r, L, R, f);
	}
	// add y = ax + b on lo <= x < hi (default: whole line)
	void add(ll a, ll b, ll lo = LLONG_MIN, ll hi = LLONG_MAX){ _add(1, 0, n, _id(lo), _id(hi), {a, b}); }
	ll query(ll x){ // x must be in xs; LLONG_MAX if no line covers x
		ll res = LLONG_MAX; int k = 1, l = 0, r = n, i = _id(x);
		while (true){
			res = min(res, t[k](x));
			if (r - l == 1) return res;
			int m = (l + r) / 2;
			if (i < m) k = 2 * k, r = m; else k = 2 * k + 1, l = m;
		}
	}
};
