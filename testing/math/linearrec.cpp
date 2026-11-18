#include "../../tex/contents/math/linearrec.hpp"
#include <cassert>

int main(){
	assert(berlekampMassey({}).empty());
	assert(berlekampMassey({0, 0, 0}).empty());
	assert((berlekampMassey({0, 1, 1, 3, 5, 11})
		== vector<MintN>{1, 2}));
	assert((berlekampMassey({0, 1, 1, 2, 3, 5, 8, 13})
		== vector<MintN>{1, 1}));
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n; cin >> n;
	vector<MintN> s(n);
	for(MintN &x: s) cin >> x;
	vector<MintN> c = berlekampMassey(s);
	cout << sz(c) << '\n';
	FOR(i, 0, sz(c) - 1){
		if(i) cout << ' ';
		cout << c[i];
	}
	cout << '\n';
}
