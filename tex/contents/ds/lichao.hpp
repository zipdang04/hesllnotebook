#pragma once
#include "../contest/template.hpp"

// Li Chao tree: min over given query points (offline),
// closed segments. addLine O(log n), addSegment O(log^2 n),
// get O(log n) (x must be in pts). Depth O(log n).
struct LiChao{
	static constexpr ll oo = LLONG_MAX;
	struct Line{
		ll a, b;
		ll eval(ll x){ return a * x + b; }
	};
	int n;
	vector<ll> xs;
	vector<Line> tree;
	LiChao(vector<ll> pts): xs(move(pts)){
		sort(xs.begin(), xs.end());
		xs.erase(unique(xs.begin(), xs.end()), xs.end());
		n = xs.size(); tree.assign(4 * n, {0, oo});
	}
	void insert(int id, int lo, int hi, Line f){
		int mid = (lo + hi) / 2;
		Line &g = tree[id];
		if(f.eval(xs[mid]) < g.eval(xs[mid])) swap(f, g);
		if(lo == hi) return;
		if(f.eval(xs[lo]) < g.eval(xs[lo]))
			insert(2 * id, lo, mid, f);
		else if(f.eval(xs[hi]) < g.eval(xs[hi]))
			insert(2 * id + 1, mid + 1, hi, f);
	}
	void add(int id, int lo, int hi, int i, int j, Line f){
		if(j < lo || hi < i) return;
		if(i <= lo && hi <= j) return insert(id, lo, hi, f);
		int mid = (lo + hi) / 2;
		add(2 * id, lo, mid, i, j, f);
		add(2 * id + 1, mid + 1, hi, i, j, f);
	}
	void addLine(ll a, ll b){ insert(1, 0, n - 1, {a, b}); }
	void addSegment(ll a, ll b, ll l, ll r){
		auto b0 = xs.begin(), e0 = xs.end();
		int i = lower_bound(b0, e0, l) - b0;
		int j = upper_bound(b0, e0, r) - b0 - 1;
		if(i > j) return;
		add(1, 0, n - 1, i, j, {a, b});
	}
	ll get(ll x){
		int id = 1, lo = 0, hi = n - 1;
		auto b0 = xs.begin(), e0 = xs.end();
		int p = lower_bound(b0, e0, x) - b0;
		ll res = oo;
		while(true){
			minimize(res, tree[id].eval(x));
			if(lo == hi) return res;
			int mid = (lo + hi) / 2;
			if(p <= mid) id = 2 * id, hi = mid;
			else id = 2 * id + 1, lo = mid + 1;
		}
	}
};
