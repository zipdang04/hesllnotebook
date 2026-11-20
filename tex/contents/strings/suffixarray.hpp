#pragma once
#include "../contest/template.hpp"

// Suffix array of the ROTATIONS of s + LCP, O(n log n),
// all arrays 0-based. Suffixes: SuffixArray(s + '$').
// LCP of ranks a < b = min lcp[a+1..b].
struct SuffixArray{
	vector<int> sa, rnk, lcp;
	SuffixArray(const string &s){
		int n = sz(s), classes = 256;
		sa = rnk = lcp = vector<int>(n);
		vector<int> x(n), y(n), cnt(max(n, 256));
		FOR(i, 0, n-1){ x[i] = (unsigned char)s[i]; y[i] = i; }
		// y holds starts ordered by their second half.
		for(int k = 0; k < n; k = max(1, 2 * k)){
			if(k)
				FOR(i, 0, n - 1)
					y[i] = sa[i] - k + (sa[i] < k ? n : 0);
			fill(cnt.begin(), cnt.begin() + classes, 0);
			FOR(i, 0, n - 1) cnt[x[y[i]]]++;
			FOR(i, 1, classes - 1) cnt[i] += cnt[i-1];
			REV(i, n - 1, 0) sa[--cnt[x[y[i]]]] = y[i];
			swap(x, y); classes = 1; x[sa[0]] = 0;
			FOR(i, 1, n - 1){
				int a = sa[i-1], b = sa[i];
				classes += y[a] != y[b]
					|| y[a+k < n ? a+k : a+k-n]
					!= y[b+k < n ? b+k : b+k-n];
				x[b] = classes - 1;
			}
			if(classes == n) break;
		}
		// equal rotations in increasing start
		fill(all(cnt), 0); FOR(i, 0, n - 1) cnt[x[i]]++;
		FOR(i, 1, classes - 1) cnt[i] += cnt[i-1];
		REV(i, n-1, 0){ rnk[i] = --cnt[x[i]]; sa[rnk[i]] = i; }
		string t = s + s;
		for(int i = 0, k = 0; i < n; i++){
			if(!rnk[i]){ k = 0; continue; }
			int j = sa[rnk[i]-1];
			while(k < n && t[i+k] == t[j+k]) k++;
			lcp[rnk[i]] = k; k -= k > 0;
		}
	}
};
