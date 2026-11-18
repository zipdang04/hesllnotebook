#include "../../tex/contents/strings/ahocorasick.hpp"
#include <cassert>

int main(){
	Aho a, b;
	a.add("ab"); a.add("b"); a.build();
	b.add("a"); b.build();
	assert(a.countMatch("abab") == 4);
	assert(b.countMatch("abab") == 2);
	a = Aho();
	assert(sz(a.t) == 1 && a.order.empty());
	assert(b.countMatch("abab") == 2);
	Aho empty; empty.build();
	assert(empty.countMatch("abc") == 0);
	Aho big;
	FOR(i, 1, 50000) big.add("a");
	big.build();
	assert(big.countMatch(string(50000, 'a'))
		== 2500000000LL);
	Aho trie;
	int end = trie.add("abc"), other = trie.add("b");
	assert(end == 3 && other == 4 && sz(trie.t) == 5);
	FOR(u, 0, 4) FOR(c, 0, Aho::K - 1){
		int v = 0;
		if(u == 0 && c == 0) v = 1;
		if(u == 1 && c == 1) v = 2;
		if(u == 2 && c == 2) v = 3;
		if(u == 0 && c == 1) v = 4;
		assert(trie.t[u].nxt[c] == v);
	}
	trie.build();
	assert((trie.order == vector<int>{0, 1, 4, 2, 3}));
	mt19937 rng(12345);
	FOR(trial, 1, 10000){
		string s(rng() % 21, 'a');
		for(char &c: s) c += rng() % 3;
		int n = 1 + rng() % 8;
		vector<string> p(n);
		vector<int> e(n), count(n), first(n, -1);
		Aho aho;
		FOR(i, 0, n - 1){
			p[i] = string(1 + rng() % 6, 'a');
			for(char &c: p[i]) c += rng() % 3;
			e[i] = aho.add(p[i]);
		}
		aho.build();
		int nodes = sz(aho.t);
		assert(sz(aho.order) == nodes);
		assert(aho.order[0] == 0);
		vector<int> seen(nodes), hit(nodes);
		for(int u: aho.order){
			assert(0 <= u && u < nodes);
			assert(!seen[u]++);
		}
		int total = 0;
		FOR(i, 0, n - 1){
			int pos = (int)s.find(p[i]);
			while(pos != -1){
				count[i]++;
				pos = (int)s.find(p[i], pos + 1);
			}
			total += count[i];
		}
		assert(aho.countMatch(s) == total);
		int u = 0;
		FOR(j, 0, sz(s) - 1){
			u = aho.t[u].nxt[s[j] - Aho::FIRST];
			hit[u]++;
			int ends = 0;
			FOR(i, 0, n - 1){
				int start = j - sz(p[i]) + 1;
				if(start >= 0 && s.compare(start,
						sz(p[i]), p[i]) == 0) ends++;
				for(int v = u; v; v = aho.t[v].link)
					if(v == e[i] && first[i] == -1)
						first[i] = start;
			}
			assert(aho.t[u].cnt == ends);
		}
		REV(i, nodes - 1, 1){
			int v = aho.order[i];
			hit[aho.t[v].link] += hit[v];
		}
		FOR(i, 0, n - 1){
			assert(hit[e[i]] == count[i]);
			int pos = (int)s.find(p[i]);
			assert(first[i] == pos);
		}
	}
	return 0;
}
