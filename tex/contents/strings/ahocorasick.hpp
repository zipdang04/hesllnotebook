#pragma once
#include "../contest/template.hpp"

// Aho-Corasick over K letters from FIRST; node 0 = root.
// O(total length * K) build, O(1) per scanned character.
struct Aho{
	// ---- EDIT: alphabet ----
	static const int K = 26; static const char FIRST = 'a';
	// ---- end of EDIT ----
	struct Node{ int nxt[K] = {}, link = 0; ll cnt = 0; };
	vector<Node> t;
	vector<int> order; // BFS order, filled by build()
	Aho(): t(1){}
	// p's end node; cnt++ there
	int add(const string &p){
		int u = 0;
		for(char ch: p){
			if(!t[u].nxt[ch - FIRST]){
				t[u].nxt[ch - FIRST] = sz(t); t.push_back({});
			}
			u = t[u].nxt[ch - FIRST];
		}
		t[u].cnt++;
		return u;
	}
	// links, goto table, cnt += cnt[link]
	void build(){
		order = {0};
		for(int i = 0; i < sz(order); i++){
			int u = order[i];
			FOR(c, 0, K - 1){
				int v = t[u].nxt[c];
				int w = u ? t[t[u].link].nxt[c] : 0;
				if(!v){
					t[u].nxt[c] = w; continue;
				}
				t[v].link = w; t[v].cnt += t[w].cnt;
				order.push_back(v);
			}
		}
	}
	// Total occurrences of all patterns.
	// Length L ending at 0-based j starts at j - L + 1.
	ll countMatch(const string &s){
		ll r = 0; int u = 0;
		for(char c: s) r += t[u = t[u].nxt[c - FIRST]].cnt;
		return r;
	}
};
