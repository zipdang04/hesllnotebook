#include "../../tex/contents/math/gauss.hpp"
#include <cassert>

typedef vector<MintN> Row;
bool check(const vector<Row> &a, const Row &x,
		const Row &b){
	FOR(i, 0, sz(a) - 1){
		MintN sum = 0;
		FOR(j, 0, sz(x) - 1) sum += a[i][j] * x[j];
		if(sum != b[i]) return false;
	}
	return true;
}
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(solveLinear<998244353>({{1, 1}, {1, 1}},
		{1, 2}).empty());
	vector<Row> a1 = {{1, 1, 1}};
	auto s1 = solveLinear(a1, Row{1});
	assert(sz(s1) == 3 && check(a1, s1[0], {1}));
	FOR(k, 1, 2) assert(check(a1, s1[k], {0}));
	auto s2 = solveLinear<998244353>({{1, 0, 0}}, {5});
	assert(sz(s2) == 3 && s2[0][0] == 5);
	assert(s2[1][1] == 1 && s2[2][2] == 1);
	vector<Row> zero(2);
	assert(solveLinear(zero, Row{0, 0}) == vector<Row>(1));
	assert(solveLinear(zero, Row{0, 1}).empty());
	int n, m;
	cin >> n >> m;
	vector<Row> a(n, Row(m));
	Row b(n);
	for(auto &row: a) for(auto &x: row) cin >> x;
	for(auto &x: b) cin >> x;
	auto s = solveLinear(a, b);
	if(s.empty()){
		cout << -1 << '\n';
		return 0;
	}
	assert(check(a, s[0], b));
	cout << sz(s) - 1 << '\n';
	for(auto &v: s){
		for(auto &x: v) cout << x << ' ';
		cout << '\n';
	}
}
