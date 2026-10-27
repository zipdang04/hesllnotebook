#pragma once
#include "../contest/template.hpp"

struct Hamming{
	int n; vector<int> pos; // pos[i] = codeword index of data bit i
	Hamming(int n): n(n){ EFOR(int, i, 3, n) if (i & (i - 1)) pos.push_back(i); }
	vector<int> encode(const vector<int> &d){ // d.size() == pos.size()
		vector<int> c(n); int s = 0, p = 0;
		EFOR(int, i, 0, (int)pos.size()) if (d[i]) c[pos[i]] = 1, s ^= pos[i], p ^= 1;
		for (int j = 1; j < n; j *= 2) p ^= c[j] = (s & j) > 0;
		c[0] = p; return c;
	}
	// 0 clean, 1 one flip fixed in c, 2 two flips (c untouched); d = data bits of c
	int decode(vector<int> &c, vector<int> &d){
		int s = 0, p = 0;
		EFOR(int, i, 0, n) if (c[i]) s ^= i, p ^= 1;
		if (p) c[s] ^= 1;
		d.clear(); for (int i: pos) d.push_back(c[i]);
		return p ? 1 : s ? 2 : 0;
	}
};
