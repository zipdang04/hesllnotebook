#pragma once
#include "../contest/template.hpp"

// Online Eertree (palindromic tree), K letters from FIRST.
// Nodes 0, 1: length -1, empty; O(n) amortised, n + 2 max.
struct Eertree{
	// ---- EDIT: alphabet ----
	static const int K = 26;
	static const char FIRST = 'a';
	// ---- end of EDIT ----
	struct Node{
		int nxt[K] = {}, link = 0, len = 0, cnt = 0, dep = 0;
	};
	vector<Node> t;
	string s; // the characters added so far
	int last = 1;
	Eertree(): t(2){ t[0].len = -1; }
	// longest suffix-palindrome of v extendable by s[i]
	int getLink(int v, int i){
		while(i - t[v].len - 1 < 0
				|| s[i - t[v].len - 1] != s[i]) v = t[v].link;
		return v;
	}
	// node of the longest palindromic suffix; ends at
	// i = sz(s) - 1, starts at i - len + 1
	int add(char ch){
		s += ch; int i = sz(s) - 1, c = ch - FIRST;
		int p = getLink(last, i);
		if(!t[p].nxt[c]){
			// A length-1 palindrome links to the empty root.
			int q = max(1, t[getLink(t[p].link, i)].nxt[c]);
			t.push_back({}); t.back().len = t[p].len + 2;
			t.back().link = q; t.back().dep = t[q].dep + 1;
			t[p].nxt[c] = sz(t) - 1;
		}
		last = t[p].nxt[c]; t[last].cnt++;
		return last;
	}
};
