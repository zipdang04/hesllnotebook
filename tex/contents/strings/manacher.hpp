#pragma once
#include "../contest/template.hpp"

// Palindrome lengths at centres 0..2n-2, O(n).
vector<int> manacher(const string &s){
	int n = sz(s), l = 0, r = -1;
	vector<int> p(max(0, 2 * n - 1));
	FOR(i, 0, sz(p) - 1){
		int a = (i + 1) / 2, b = i / 2;
		int k = b >= r ? 0 : min(p[2*(l+r)-i] / 2, r - b);
		while(a - k > 0 && b + k + 1 < n
				&& s[a-k-1] == s[b+k+1]) k++;
		p[i] = 2 * k + 1 - i % 2;
		if(b + k > r) l = a - k, r = b + k;
	}
	return p;
}
