#include "../../tex/contents/ds/treap.hpp"

struct AddMin{ // range add, range min
	using S = ll; using F = ll;
	static S op(S x, S y){ return min(x, y); }
	static S e(){ return LLONG_MAX; }
	static S mp(F f, S x){ return x + f; }
	static F cp(F f, F g){ return f + g; }
	static F id(){ return 0; }
};
int main(){
	mt19937 rng(1);
	auto rnd = [&](ll a, ll b){ return uniform_int_distribution<ll>(a, b)(rng); };
	EFOR(int, it, 0, 3000){
		int n = it % 3 == 0 ? 0 : rnd(1, it < 2000 ? 8 : 60);
		vector<ll> b(n); for (ll& x: b) x = rnd(-1e9, 1e9);
		Treap<AddMin> tr = n || rnd(0, 1) ? Treap<AddMin>(b) : Treap<AddMin>();
		auto fail = [&](const char* w){ printf("FAIL %s it=%d n=%d\n", w, it, n); exit(1); };
		EFOR(int, op, 0, 300){
			int m = b.size(), ty = rnd(0, 5);
			if (tr.size() != m) fail("size");
			if (ty == 0 || m == 0){ int i = rnd(1, m + 1); ll x = rnd(-1e9, 1e9); tr.insert(i, x); b.insert(b.begin() + i - 1, x); continue; }
			if (ty == 1){ int i = rnd(1, m); tr.erase(i); b.erase(b.begin() + i - 1); continue; }
			int L = rnd(1, m), R = rnd(L, m);
			if (ty == 2){ tr.reverse(L, R); std::reverse(b.begin() + L - 1, b.begin() + R); }
			else if (ty == 3){ ll f = rnd(-1e9, 1e9); tr.apply(L, R, f); FOR(int, i, L, R) b[i - 1] += f; }
			else if (ty == 4){ ll mn = LLONG_MAX; FOR(int, i, L, R) mn = min(mn, b[i - 1]); if (tr.query(L, R) != mn) fail("query"); }
			else { L = rnd(1, m + 1); if (tr.query(L, L - 1) != LLONG_MAX) fail("empty"); }
		}
		FOR(int, i, 1, (int)b.size()) if (tr.query(i, i) != b[i - 1]) fail("final");
	}
	puts("OK");
}
