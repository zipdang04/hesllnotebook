#include "../../tex/contents/math/modint.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	const ll P = 998244353;
	assert(MintN().v == 0 && MintN(-1).v == P - 1);
	assert(MintN(LLONG_MIN).v == (LLONG_MIN % P + P) % P);
	assert(MintN(1LL << 62).v == (1LL << 62) % P);
	for(ll x: {1, 2, 998244352, 123456789}){
		MintN a = x;
		assert((a / a).v == 1 && a * a.inv() == 1);
		assert(a.pow(0).v == 1 && a.pow(P - 1) == 1);
		assert(2 * a == a + a && a - a == 0 && a + -a == 0);
		assert(1 / a == a.inv());
	}
	assert(MintN(2).pow(3) == 8 && MintN(0).pow(0) == 1);
	assert(MintN(0).pow(1) == 0);
	assert(MintN(-1).pow(LLONG_MAX) == MintN(-1));
	assert((-MintN(0)).v == 0);
	assert((MintN(1) - MintN(2)).v == P - 1);
	assert(MintN(P - 1) * MintN(P - 1) == 1);
	MintN x = 5;
	x += 7;
	x -= 20;
	x *= 3;
	x /= 3;
	assert(x == -8 && x != 8);
	istringstream is("-5 1000000000000");
	MintN p, q;
	is >> p >> q;
	assert(p.v == P - 5 && q.v == 1000000000000LL % P);
	ostringstream os;
	os << p << ' ' << q;
	assert(os.str() == "998244348 " + to_string(q.v));
	Mint<1000000007> y = 1000000006;
	assert((y + 1).v == 0);
	assert(Mint<2>(3).v == 1 && Mint<2>(1).inv().v == 1);
	for(MintN z: vector<MintN>(3)) assert(z.v == 0);
	int n;
	cin >> n;
	vector<vector<MintN>> a(n, vector<MintN>(2 * n));
	FOR(i, 0, n - 1){
		FOR(j, 0, n - 1) cin >> a[i][j];
		a[i][n + i] = 1;
	}
	FOR(c, 0, n - 1){
		int r = c;
		while(r < n && a[r][c] == 0) r++;
		if(r == n){
			cout << -1 << '\n';
			return 0;
		}
		swap(a[r], a[c]);
		MintN iv = a[c][c].inv();
		for(MintN &e: a[c]) e *= iv;
		FOR(i, 0, n - 1) if(i != c){
			MintN f = a[i][c];
			FOR(j, 0, 2 * n - 1) a[i][j] -= f * a[c][j];
		}
	}
	FOR(i, 0, n - 1)
		FOR(j, 0, n - 1)
			cout << a[i][n + j] << " \n"[j == n - 1];
}
