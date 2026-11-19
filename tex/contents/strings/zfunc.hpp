#pragma once
#include "../contest/template.hpp"

// z[i] = longest common prefix of s and s.substr(i);
// 0-based, z[0] = n. O(n).
vector<int> zFunc(const string &s){
	int n = sz(s), l = -1, r = -1;
	vector<int> z(n);
	FOR(i, 1, n - 1){
		z[i] = i >= r ? 0 : min(r - i, z[i - l]);
		while(i + z[i] < n && s[i + z[i]] == s[z[i]]) z[i]++;
		if(i + z[i] > r) l = i, r = i + z[i];
	}
	if(n) z[0] = n;
	return z;
}
