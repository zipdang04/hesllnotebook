#include "../../tex/contents/strings/suffixarray.hpp"
#include <cassert>

int main(){
	mt19937 rng(12345);
	FOR(trial, 1, 100000){
		int n = rng() % 11, alphabet = rng() % 3 + 1;
		string s(n, 'a');
		for(char &c: s) c += rng() % alphabet;
		vector<string> rot(n);
		vector<int> order(n), rank(n), lcp(n);
		FOR(i, 0, n - 1){
			rot[i] = s.substr(i) + s.substr(0, i);
			order[i] = i;
		}
		stable_sort(all(order), [&](int a, int b){
			return rot[a] < rot[b];
		});
		FOR(i, 0, n - 1) rank[order[i]] = i;
		FOR(i, 1, n - 1){
			string &a = rot[order[i-1]];
			string &b = rot[order[i]];
			while(lcp[i] < n && a[lcp[i]] == b[lcp[i]])
				lcp[i]++;
		}
		SuffixArray sa(s);
		assert(sa.sa == order);
		assert(sa.rnk == rank);
		assert(sa.lcp == lcp);
		vector<int> suffix(n);
		FOR(i, 0, n - 1) suffix[i] = i;
		sort(all(suffix), [&](int a, int b){
			return s.substr(a) < s.substr(b);
		});
		SuffixArray withEnd(s + '$');
		assert(withEnd.sa[0] == n);
		FOR(i, 0, n - 1)
			assert(withEnd.sa[i+1] == suffix[i]);
	}
	return 0;
}
