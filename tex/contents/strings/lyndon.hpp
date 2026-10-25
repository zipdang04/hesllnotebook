#pragma once
#include "../contest/template.hpp"

// returns 0 = a_0 < ... < a_k = n; s[a_i, a_{i+1}) are the Lyndon factors, non-increasing
template<class S> vector<int> duval(const S &s){
	int n = s.size(), i = 0; vector<int> a{0};
	while (i < n){
		int j = i + 1, k = i;
		for (; j < n && s[k] <= s[j]; j++) k = s[k] < s[j] ? i : k + 1;
		for (; i <= k; i += j - k) a.push_back(i + j - k);
	}
	return a;
}
// some r such that rotate(s.begin(), s.begin() + r, s.end()) is the smallest rotation; n >= 1
int minRotation(const string &s){
	auto a = duval(s + s); return *prev(lower_bound(a.begin(), a.end(), (int)s.size()));
}
