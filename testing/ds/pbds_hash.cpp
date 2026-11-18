#include "../../tex/contents/ds/pbds.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	{
		CustomHash h;
		uint64_t before = CURRENT_TIMESTAMP();
		uint64_t seed = h(0);
		uint64_t after = CURRENT_TIMESTAMP();
		seed ^= seed >> 31 ^ seed >> 62;
		seed *= 0x319642b2d24d8ec3;
		seed ^= seed >> 27 ^ seed >> 54;
		seed *= 0x96de1b173f119089;
		seed ^= seed >> 30 ^ seed >> 60;
		seed -= 0x9e3779b97f4a7c15;
		assert(before <= seed && seed <= after);
		assert(h(5) == h(5) && CustomHash()(5) == h(5));
		gp_hash_table<ll, ll, CustomHash> mp2;
		mp2[-3] = 4;
		assert(mp2.find(-3)->se == 4);
		assert(mp2.find(7) == mp2.end());
		unordered_map<ll, int, CustomHash> um;
		um[1] = 2;
		assert(um.find(1)->second == 2);
	}
	gp_hash_table<ll, ll, CustomHash> mp;
	int q;
	cin >> q;
	FOR(i, 1, q){
		int t;
		ll k, v;
		cin >> t >> k;
		if(t == 0){
			cin >> v;
			mp[k] = v;
		}else{
			auto it = mp.find(k);
			cout << (it == mp.end() ? 0 : it->se) << '\n';
		}
	}
}
