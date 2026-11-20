#pragma once
#include "../contest/template.hpp"

// Mo's algorithm: answers closed queries [l, r] (1-based)
// offline, O((n + q) sqrt q) add/del calls.
// add(i, right): a[i] joins the window at its right end if
// right, else at its left end; del(i, right): it leaves.
// Call: moQueries(q, add, del, [&]{ return cur; });
template<class Add, class Del, class Get>
vector<ll> moQueries(const vector<pair<int, int>> &q,
		Add add, Del del, Get get){
	int nq = sz(q), n = 0, L = 1, R = 0;
	vector<ll> ans(nq);
	if(!nq) return ans;
	for(auto &x: q) maximize(n, x.se);
	int blk = max(1, (int)(n / sqrt(nq)));
	vector<int> ord(nq); iota(all(ord), 0);
	sort(all(ord), [&](int i, int j){
		int bi = q[i].fi / blk, bj = q[j].fi / blk;
		if(bi != bj) return bi < bj;
		return bi & 1 ? q[i].se > q[j].se : q[i].se < q[j].se;
	});
	for(int i: ord){
		auto [l, r] = q[i];
		while(R < r) add(++R, 1); while(L > l) add(--L, 0);
		while(R > r) del(R--, 1); while(L < l) del(L++, 0);
		ans[i] = get();
	}
	return ans;
}
