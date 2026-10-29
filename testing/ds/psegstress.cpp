#include "../../tex/contents/ds/ps_seg.hpp"
int main(){
	mt19937 rng(1);
	FOR(int, seed, 1, 3000){
		int n = rng() % 12 + 1;
		auto mx = [](ll a, ll b){ return max(a, b); };
		PSeg<ll> s(n);
		PSeg<ll, decltype(mx)> m(n, -(ll)4e18, mx);
		vector<vector<ll>> ver{vector<ll>(n + 1, 0)}, vm{vector<ll>(n + 1, -(ll)4e18)};
		vector<int> rs{0}, rm{0};
		auto fail = [&](){ printf("FAIL seed %d\n", seed); exit(1); };
		FOR(int, op, 0, 59){
			int k = rng() % ver.size();
			if (rng() % 3){
				int p = rng() % n + 1; ll x = (ll)(rng() % 2001) * 1000000000 - 1000000000000ll;
				ver.push_back(ver[k]); ver.back()[p] = x; rs.push_back(s.upd(rs[k], p, x));
				vm.push_back(vm[k]); vm.back()[p] = x; rm.push_back(m.upd(rm[k], p, x));
			}
			FOR(int, i, 0, (int)ver.size() - 1){ // every version, so old ones must be untouched
				int a = rng() % (n + 1) + 1, b = rng() % (n + 1) + 1;
				if (rng() % 5 == 0) a = b + 1; // empty range
				else if (rng() % 5 == 1) a = 1, b = 1; // single first
				else if (rng() % 5 == 2) a = n, b = n; // single last
				else if (a > b) swap(a, b);
				if (b > n && a <= n) b = n;
				if (a > n) a = n + 1, b = n;
				ll es = 0, em = -(ll)4e18;
				FOR(int, j, a, b) es += ver[i][j], em = max(em, vm[i][j]);
				if (s.qry(rs[i], a, b) != es || m.qry(rm[i], a, b) != em) fail();
			}
		}
	}
	puts("OK");
}
