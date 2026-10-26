#include "../../tex/contents/ds/ps_seg.hpp"
int main(){
	mt19937 rng(1);
	FOR(int, seed, 1, 3000){
		int n = rng() % 12 + 1;
		auto mx = [](ll a, ll b){ return max(a, b); };
		PSeg<ll> s(n);
		PSeg<ll, decltype(mx)> m(n, -(ll)4e18, mx);
		vector<vector<ll>> ver{vector<ll>(n, 0)}, vm{vector<ll>(n, -(ll)4e18)};
		vector<int> rs{0}, rm{0};
		auto fail = [&](){ printf("FAIL seed %d\n", seed); exit(1); };
		EFOR(int, op, 0, 60){
			int k = rng() % ver.size();
			if (rng() % 3){
				int p = rng() % n; ll x = (ll)(rng() % 2001) * 1000000000 - 1000000000000ll;
				ver.push_back(ver[k]); ver.back()[p] = x; rs.push_back(s.upd(rs[k], p, x));
				vm.push_back(vm[k]); vm.back()[p] = x; rm.push_back(m.upd(rm[k], p, x));
			}
			EFOR(int, i, 0, (int)ver.size()){ // every version, so old ones must be untouched
				int a = rng() % (n + 1), b = rng() % (n + 1); if (a > b) swap(a, b);
				ll es = 0, em = -(ll)4e18;
				EFOR(int, j, a, b) es += ver[i][j], em = max(em, vm[i][j]);
				if (s.qry(rs[i], a, b) != es || m.qry(rm[i], a, b) != em) fail();
			}
		}
	}
	puts("OK");
}
