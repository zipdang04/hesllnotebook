#pragma once
#include "../contest/template.hpp"

// Suffix automaton, online, K letters from FIRST. Node 0 =
// root, link -1; at most 2n nodes. O(n) amortised.
struct SAM{
	// ---- EDIT: alphabet ----
	static const int K = 26; static const char FIRST = 'a';
	// ---- end of EDIT ----
	// pos = end of the first occurrence; start pos - L + 1
	struct Node{
		int nxt[K] = {}, link = -1, len = 0, pos = 0;
	};
	vector<Node> t;
	int last = 0;
	SAM(): t(1){}
	void add(char ch){
		int c = ch - FIRST, k = sz(t), p = last;
		t.push_back({});
		t[k].len = t[last].len + 1;
		t[k].pos = t[k].len - 1; last = k;
		for(; p != -1 && !t[p].nxt[c]; p = t[p].link)
			t[p].nxt[c] = k;
		if(p == -1){ t[k].link = 0; return; }
		int q = t[p].nxt[c];
		if(t[p].len + 1 == t[q].len){
			t[k].link = q; return;
		}
		int w = sz(t); t.push_back(t[q]);
		t[w].len = t[p].len + 1;
		for(; p != -1 && t[p].nxt[c] == q; p = t[p].link)
			t[p].nxt[c] = w;
		t[q].link = t[k].link = w;
	}
};
