#include "../../tex/contents/math/gauss.hpp"
#include <cassert>

bool dot(const BitRow &row, const BitRow &x){
	return (row & x).count() % 2;
}
int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	auto s0 = solveXor({}, {}, 3);
	assert(sz(s0) == 4 && s0[0].none());
	FOR(k, 1, 3) assert(s0[k].count() == 1 && s0[k][k - 1]);
	BitRow all01;
	all01[0] = all01[1] = 1;
	assert(solveXor({all01, all01}, {1, 0}, 3).empty());
	BitRow r1, r2;
	r1[0] = r1[2] = 1;
	r2[1] = r2[2] = 1;
	auto s1 = solveXor({r1, r2}, {1, 1}, 3);
	assert(sz(s1) == 2);
	assert(dot(r1, s1[0]) == 1 && dot(r2, s1[0]) == 1);
	assert(dot(r1, s1[1]) == 0 && dot(r2, s1[1]) == 0);
	for(auto &v: s1) assert((v >> 3).none());
	vector<BitRow> zero(2);
	assert(solveXor(zero, {0, 0}, 0) == vector<BitRow>(1));
	assert(solveXor(zero, {0, 1}, 0).empty());
	int n, m;
	cin >> n >> m;
	vector<BitRow> a(n);
	vector<int> b(n);
	string t;
	FOR(i, 0, n - 1){
		cin >> t;
		FOR(j, 0, m - 1) a[i][j] = t[j] == '1';
	}
	cin >> t;
	FOR(i, 0, n - 1) b[i] = t[i] - '0';
	auto s = solveXor(a, b, m);
	if(s.empty()){
		cout << -1 << '\n';
		return 0;
	}
	FOR(i, 0, n - 1) assert(dot(a[i], s[0]) == b[i]);
	cout << sz(s) - 1 << '\n';
	for(auto &v: s){
		FOR(j, 0, m - 1) cout << v[j];
		cout << '\n';
	}
}
