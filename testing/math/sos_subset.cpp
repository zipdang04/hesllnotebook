#include "../../tex/contents/math/sos.hpp"
#include "../../tex/contents/math/modint.hpp"

// subset convolution: subset zeta per popcount, ranked
// product, subset inverse, keep rank |S|
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n;
	cin >> n;
	int N = 1 << n;
	vector<vector<MintN>> f(n + 1, vector<MintN>(N));
	auto g = f, h = f;
	FOR(i, 0, N - 1) cin >> f[__builtin_popcount(i)][i];
	FOR(i, 0, N - 1) cin >> g[__builtin_popcount(i)][i];
	auto add = [](MintN &x, MintN y){ x += y; };
	FOR(k, 0, n) sos(f[k], add), sos(g[k], add);
	FOR(k, 0, n) FOR(i, 0, k) FOR(S, 0, N - 1)
		h[k][S] += f[i][S] * g[k-i][S];
	FOR(k, 0, n) sos(h[k], [](MintN &x, MintN y){ x -= y; });
	FOR(S, 0, N - 1)
		cout << h[__builtin_popcount(S)][S]
			<< " \n"[S == N - 1];
}
