#include "../../tex/contents/strings/suffixarray.hpp"
#include <cassert>

int main(){
	string bytes = {0, (char)255, 0, (char)254, 0, (char)253};
	SuffixArray binary(bytes);
	assert((binary.sa == vector<int>{4, 2, 0, 5, 3, 1}));
	int common = 0;
	while(common < sz(bytes) &&
		bytes[(binary.sa[0]+common) % 6] ==
		bytes[(binary.sa[2]+common) % 6]) common++;
	assert(common == 1);
	assert(common == min(binary.lcp[1], binary.lcp[2]));
	SuffixArray empty("");
	assert(empty.sa.empty() && empty.rnk.empty()
		&& empty.lcp.empty());
	SuffixArray one("a");
	assert(one.sa == vector<int>{0});
	assert(one.rnk == vector<int>{0});
	assert(one.lcp == vector<int>{0});
	SuffixArray equal("aaaa");
	assert((equal.sa == vector<int>{0, 1, 2, 3}));
	assert((equal.lcp == vector<int>{0, 4, 4, 4}));
	SuffixArray joined("aa#a$");
	assert(joined.sa[0] == 2);
	int best = 0;
	FOR(i, 1, 4){
		int a = joined.sa[i-1], b = joined.sa[i];
		if((a < 2 && b == 3) || (b < 2 && a == 3))
			maximize(best, joined.lcp[i]);
	}
	assert(best == 1);
	SuffixArray x("ab"); x = SuffixArray("ba");
	assert((x.sa == vector<int>{1, 0}));
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	int n = sz(s);
	SuffixArray sa(s + '$');
	assert(sa.sa[0] == n);
	FOR(i, 0, n)
		assert(sa.rnk[sa.sa[i]] == i);
	FOR(i, 1, n)
		cout << sa.sa[i] << (i == n ? '\n' : ' ');
}
