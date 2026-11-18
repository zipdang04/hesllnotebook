#include "../../tex/contents/strings/eertree.hpp"
#include <cassert>

bool isPal(string s){
	string r = s;
	reverse(r.begin(), r.end());
	return s == r;
}

void check(const string &s){
	Eertree et;
	vector<string> word(2);
	map<string, int> occurrences;
	assert(sz(et.t) == 2 && et.s.empty());
	assert(et.t[0].len == -1 && et.t[1].len == 0);
	FOR(i, 0, sz(s) - 1){
		int best = 0, count = 0;
		FOR(len, 1, i + 1){
			string w = s.substr(i - len + 1, len);
			if(!isPal(w)) continue;
			occurrences[w]++;
			best = len;
			count++;
		}
		int v = et.add(s[i]);
		assert(et.s == s.substr(0, i + 1));
		assert(v == et.last && v >= 2 && v < sz(et.t));
		assert(et.t[v].len == best);
		assert(et.t[v].dep == count);
		string w = s.substr(i - best + 1, best);
		assert(isPal(w));
		if(v == sz(word)) word.push_back(w);
		assert(v < sz(word) && word[v] == w);
		assert(sz(et.t) == sz(word));
		assert(sz(et.t) <= i + 3);
		assert(et.t.capacity() < 2 * et.t.size());
		assert(sz(et.t) - 2 == (int)occurrences.size());
		FOR(u, 0, sz(word) - 1){
			int len = u == 0 ? -1 : sz(word[u]);
			if(len >= 0 && (len > i ||
					s.substr(i - len, len) != word[u]))
				continue;
			int expected = 0;
			FOR(x, 1, sz(word) - 1){
				int k = sz(word[x]);
				if(k > len || i - k - 1 < 0) continue;
				if(word[u].substr(len - k) != word[x])
					continue;
				if(s[i-k-1] == s[i] &&
						k > et.t[expected].len)
					expected = x;
			}
			assert(et.getLink(u, i) == expected);
		}
	}
	FOR(v, 2, sz(word) - 1){
		string w = word[v];
		int link = 1;
		FOR(u, 2, sz(word) - 1){
			int len = sz(word[u]);
			if(len >= sz(w)) continue;
			if(w.substr(sz(w) - len) == word[u] &&
					len > sz(word[link])) link = u;
		}
		assert(et.t[v].link == link);
	}
	for(int v = sz(et.t) - 1; v >= 2; v--)
		et.t[et.t[v].link].cnt += et.t[v].cnt;
	FOR(v, 2, sz(word) - 1)
		assert(et.t[v].cnt == occurrences[word[v]]);
}

int main(){
	mt19937 rng(12345);
	check("");
	check(string(40, 'a'));
	FOR(trial, 1, 1000){
		string s(rng() % 41, 'a');
		for(char &c: s) c += rng() % 2;
		check(s);
	}
}
