#include "../../tex/contents/math/linearrec.hpp"
#include <cassert>

int main(){
	MintN a = 0, b = 1;
	FOR(k, 0, 40){
		assert(kthTerm({0, 1}, {1, 1}, k) == a);
		MintN next = a + b; a = b; b = next;
	}
	assert(kthTerm({}, {}, 5) == 0);
	assert(kthTerm({7}, {}, 0) == 0);
	assert(kthTerm({4, 5, 6}, {1, 2, 3}, 2) == 6);
	{ // a supplied initial term survives a large d
		int d = 4194305;
		vector<MintN> s(d, 1), c(d);
		s[1] = 2; c[d - 1] = 1;
		assert(kthTerm(s, c, 1) == 2);
	}
	vector<MintN> s(201), c(40);
	FOR(i, 0, 39){
		s[i] = i * i + 1;
		c[i] = i + 1;
	}
	FOR(i, 40, 200)
		FOR(j, 0, 39) s[i] += c[j] * s[i - 1 - j];
	FOR(k, 0, 200) assert(kthTerm(s, c, k) == s[k]);
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int d; ll k; cin >> d >> k;
	vector<MintN> initial(d), coef(d);
	for(MintN &x: initial) cin >> x;
	for(MintN &x: coef) cin >> x;
	cout << kthTerm(initial, coef, k) << '\n';
}
