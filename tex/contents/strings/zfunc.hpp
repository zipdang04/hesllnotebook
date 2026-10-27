#pragma once
#include "../contest/template.hpp"

// z[i] = LCP(s, s[i..]), z[0] = n
template<class S> vector<int> zFunc(const S &s){
	int n = s.size(); vector<int> z(n, n);
	for (int i = 1, l = 0, r = 0; i < n; i++){
		z[i] = i < r ? min(r - i, z[i - l]) : 0;
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] > r) l = i, r = i + z[i];
	}
	return z;
}
