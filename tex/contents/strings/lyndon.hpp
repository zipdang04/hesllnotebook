#pragma once
#include "../contest/template.hpp"

// Lyndon factorisation by Duval: s = w1 w2 .. with w1 >= w2
// >= .. Lyndon words. Returns 0-based starts. O(n).
vector<int> duval(const string &s){
	int n = sz(s), i = 0;
	vector<int> st;
	auto *u = (const unsigned char *)s.data();
	while(i < n){
		int j = i + 1, k = i;
		for(; j < n && u[k] <= u[j]; j++)
			k = u[k] < u[j] ? i : k + 1;
		for(; i <= k; i += j - k)
			st.push_back(i);
	}
	return st;
}
