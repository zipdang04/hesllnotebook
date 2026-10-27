#include "../../tex/contents/ds/pbds.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int q; cin >> q;
	hashMap<ll, ll> h;
	string out;
	while (q--){
		int t; ll k; cin >> t >> k;
		if (t == 0) cin >> h[k];
		else{ auto it = h.find(k); out += to_string(it == h.end() ? 0 : it->se) + '\n'; }
	}
	cout << out;
}
