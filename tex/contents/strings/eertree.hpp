#pragma once
#include "../contest/template.hpp"

// node 0 = odd root (len -1), node 1 = even root (len 0); nodes are created in order of first occurrence
template<int K = 26, char first = 'a'> struct Eertree{
	struct Node{int len, link = 0, cnt = 0, nxt[K] = {};};  // nxt 0 = none
	vector<Node> t{{-1}, {0}};
	vector<int> s{-1};
	int last = 1;
	int getLink(int u){
		int i = s.size() - 1;
		while (s[i - t[u].len - 1] != s[i]) u = t[u].link;
		return u;
	}
	int add(char ch){                    // returns the node of the longest palindromic suffix
		int c = ch - first; s.push_back(c);
		int p = getLink(last);
		if (!t[p].nxt[c]){
			int q = t[getLink(t[p].link)].nxt[c];
			t.push_back({t[p].len + 2, max(q, 1)});
			t[p].nxt[c] = t.size() - 1;
		}
		t[last = t[p].nxt[c]].cnt++;
		return last;
	}
	void calc(){                         // afterwards cnt[u] = #occurrences of u in s
		for (int u = t.size() - 1; u > 1; u--) t[t[u].link].cnt += t[u].cnt;
	}
};
