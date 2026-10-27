#pragma once
#include "../contest/template.hpp"

// characters first .. first+K-1; state 0 = root (empty string); nxt = 0 means no edge
template<int K = 26, char first = 'a'> struct SuffixAutomaton{
	struct Node{int len, link, cnt, nxt[K];};
	vector<Node> t{{0, -1, 0, {}}};
	int last = 0;
	SuffixAutomaton(const string &s = ""){ t.reserve(2 * s.size() + 1); for (char ch: s) extend(ch); }

	void extend(char ch){                    // append ch (online)
		int c = ch - first, cur = t.size(), p = last;
		t.push_back({t[last].len + 1, 0, 1, {}}); last = cur;
		for (; p >= 0 && !t[p].nxt[c]; p = t[p].link) t[p].nxt[c] = cur;
		if (p < 0) return;
		int q = t[p].nxt[c];
		if (t[p].len + 1 == t[q].len){ t[cur].link = q; return; }
		Node cl = t[q]; cl.len = t[p].len + 1; cl.cnt = 0;
		t[q].link = t[cur].link = t.size(); t.push_back(cl);
		for (; p >= 0 && t[p].nxt[c] == q; p = t[p].link) t[p].nxt[c] = t[q].link;
	}
	ll distinct(){                           // # distinct non-empty substrings
		ll r = 0; for (auto &x: t) if (x.link >= 0) r += x.len - t[x.link].len;
		return r;
	}
	void countEndpos(){                      // call once, after all extends: cnt[v] = |endpos(v)|, v > 0
		vector<int> o(t.size()); iota(o.begin(), o.end(), 0);
		sort(o.begin(), o.end(), [&](int a, int b){ return t[a].len > t[b].len; });
		for (int v: o) if (v) t[t[v].link].cnt += t[v].cnt;
	}
	int find(const string &p){               // state of p, or -1 if p is not a substring
		int u = 0;
		for (char ch: p) if (!(u = t[u].nxt[ch - first])) return -1;
		return u;
	}
};
