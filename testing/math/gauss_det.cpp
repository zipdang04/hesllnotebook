#include "../../tex/contents/math/gauss.hpp"
#include <cassert>

typedef vector<vector<MintN>> Mat;
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	Mat p = {{0, 1}, {1, 0}}, q = {{2, 3}, {4, 5}};
	Mat id = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	Mat z, sing = {{1, 2}, {2, 4}};
	assert(gauss(p, 2).se == -1);
	assert(gauss(q, 2).se == -2);
	assert(gauss(id, 3) == make_pair(3, MintN(1)));
	assert(gauss(z, 0) == make_pair(0, MintN(1)));
	assert(gauss(sing, 2) == make_pair(1, MintN(0)));
	vector<vector<Mint<7>>> r = {{2, 3}, {4, 5}};
	assert(gauss(r, 2).se == 5);
	Mat full = {{2, 4}, {0, 3}};
	assert(gauss(full, 2) == make_pair(2, MintN(6)));
	assert((full == Mat{{1, 0}, {0, 1}}));
	Mat rect = {{2}, {0}};
	assert(gauss(rect, 1) == make_pair(1, MintN(2)));
	assert(gauss(z, 1) == make_pair(0, MintN(0)));
	int n;
	cin >> n;
	Mat a(n, vector<MintN>(n));
	for(auto &row: a) for(auto &x: row) cin >> x;
	cout << gauss(a, n).se << '\n';
}
