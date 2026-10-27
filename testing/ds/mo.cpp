#include "../../tex/contents/ds/mo.hpp"

int main(){
	int n, q; read(n, q);
	vector<int> a(n + 1), v(n);
	FOR(int, i, 1, n) read(a[i]), v[i - 1] = a[i];
	sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());
	int m = v.size();
	FOR(int, i, 1, n) a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1; // ranks 1..m
	vector<int> bit(m + 1); int cnt = 0; ll inv = 0;
	auto upd = [&](int i, int d){ for (; i <= m; i += i & -i) bit[i] += d; };
	auto lt = [&](int i){ int s = 0; for (i--; i > 0; i -= i & -i) s += bit[i]; return s; }; // # values < rank i
	auto lessEq = [&](int i){ return lt(i + 1); };
	vector<pair<int,int>> Q(q);
	for (auto& [l, r]: Q) read(l, r), l++; // [l, r) 0-indexed -> [l+1, r]
	auto res = mo(Q,
		[&](int i, int e){ inv += e ? cnt - lessEq(a[i]) : lt(a[i]); upd(a[i], 1); cnt++; },
		[&](int i, int e){ upd(a[i], -1); cnt--; inv -= e ? cnt - lessEq(a[i]) : lt(a[i]); },
		[&]{ return inv; });
	string out;
	for (ll x: res) out += to_string(x) + '\n';
	fputs(out.c_str(), stdout);
}
