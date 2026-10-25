#pragma once
#include "../contest/template.hpp"

// sa[i] = start of the i-th smallest suffix (cyc: rotation); rk = inverse of sa
// lcp[i] = LCP(sa[i - 1], sa[i]), lcp[0] = 0; cyc = sort rotations (no sentinel)
struct SA{
	vector<int> sa, rk, lcp;
	SA(string s, bool cyc = false){
		if (!cyc) s.push_back(0);
		int n = s.size(); vector<int> t(n), c(max(n, 256));
		sa = lcp = t; iota(sa.begin(), sa.end(), 0); rk.assign(s.begin(), s.end());
		for (int k = 0; k < n; k = max(1, 2 * k)){
			EFOR(int, i, 0, n) t[i] = (sa[i] - k + n) % n;
			fill(c.begin(), c.end(), 0);
			for (int x: rk) c[x]++;
			partial_sum(c.begin(), c.end(), c.begin());
			REV(int, i, n - 1, 0) sa[--c[rk[t[i]]]] = t[i];
			auto key = [&](int i){ return pair(rk[i], rk[(i + k) % n]); };
			t[sa[0]] = 0;
			EFOR(int, i, 1, n) t[sa[i]] = t[sa[i - 1]] + (key(sa[i - 1]) != key(sa[i]));
			swap(rk, t);
		}
		EFOR(int, i, 0, n) rk[sa[i]] = i;
		for (int i = 0, k = 0; i < n; i++, k -= k > 0){
			if (!rk[i]){ k = 0; continue; }
			int j = sa[rk[i] - 1];
			while (k < n && s[(i + k) % n] == s[(j + k) % n]) k++;
			lcp[rk[i]] = k;
		}
		if (!cyc){ sa.erase(sa.begin()); lcp.erase(lcp.begin()); rk.pop_back(); for (int &x: rk) x--; }
	}
};
