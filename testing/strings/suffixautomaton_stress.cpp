#include "../../tex/contents/strings/suffixautomaton.hpp"
#include <cassert>

void check(const string &s){
	int n = sz(s);
	SAM sam;
	for(char c: s) sam.add(c);
	int m = sz(sam.t);
	if(n >= 1) assert(m <= 2 * n);
	if(n >= 2) assert(m <= 2 * n - 1);
	vector<int> order(m), cnt(m);
	iota(all(order), 0);
	sort(all(order), [&](int a, int b){
		return sam.t[a].len > sam.t[b].len;
	});
	int created = 0, sum = 0;
	FOR(v, 1, m - 1){
		int p = sam.t[v].link;
		assert(0 <= p && p < m);
		assert(sam.t[p].len < sam.t[v].len);
		cnt[v] = sam.t[v].len == sam.t[v].pos + 1;
		created += cnt[v];
		sum += sam.t[v].len - sam.t[p].len;
	}
	assert(created == n);
	for(int v: order) if(v) cnt[sam.t[v].link] += cnt[v];
	set<string> subs;
	FOR(i, 0, n - 1)
		FOR(len, 1, n - i)
			subs.insert(s.substr(i, len));
	assert(sum == sz(subs));
	for(const string &u: subs){
		int v = 0;
		for(char c: u){
			v = sam.t[v].nxt[c - SAM::FIRST];
			assert(0 < v && v < m);
		}
		int first = s.find(u);
		assert(sam.t[v].pos == first + sz(u) - 1);
		int brute = 0;
		FOR(i, 0, n - sz(u))
			brute += s.compare(i, sz(u), u) == 0;
		assert(cnt[v] == brute);
	}
	for(string u: {string("c"), s + 'a'}){
		int v = 0;
		for(char c: u){
			v = sam.t[v].nxt[c - SAM::FIRST];
			if(!v) break;
			assert(v < m);
		}
		assert(v == 0);
	}
}

int main(){
	mt19937 rng(12345);
	check("");
	check("a");
	check("abb");
	check(string(40, 'a'));
	FOR(trial, 1, 500){
		int n = rng() % 41;
		string s(n, 'a');
		for(char &c: s) c += rng() % 2;
		check(s);
	}
}
