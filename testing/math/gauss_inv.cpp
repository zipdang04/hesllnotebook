#include "../../tex/contents/math/gauss.hpp"
#include <cassert>

typedef vector<vector<MintN>> Mat;
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	Mat a1 = {{1, 2}, {3, 4}}, b1 = a1;
	assert(matInverse(b1));
	FOR(i, 0, 1) FOR(j, 0, 1){
		MintN sum = 0;
		FOR(k, 0, 1) sum += a1[i][k] * b1[k][j];
		assert(sum == (i == j));
	}
	Mat a2 = {{1, 2}, {2, 4}};
	assert(!matInverse(a2));
	Mat zero;
	assert(matInverse(zero) && zero.empty());
	int n;
	cin >> n;
	Mat a(n, vector<MintN>(n));
	for(auto &row: a) for(auto &x: row) cin >> x;
	if(!matInverse(a)){
		cout << -1 << '\n';
		return 0;
	}
	for(auto &row: a){
		for(auto &x: row) cout << x << ' ';
		cout << '\n';
	}
}
