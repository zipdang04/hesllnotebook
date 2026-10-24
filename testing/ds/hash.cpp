#include "../../tex/contents/ds/hash.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int q; cin >> q;
	unordered_map<uint64_t, uint64_t, custom_hash> mp;
	FOR(int, _, 1, q) {
		int type; uint64_t k; cin >> type >> k;
		if (type == 0) {
			uint64_t v; cin >> v; mp[k] = v;
		} else {
			auto it = mp.find(k);
			cout << (it == mp.end() ? 0 : it -> second) << '\n';
		}
	}
}
