#include "../../tex/contents/ds/pbds.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	ordSet<int> s;
	EFOR(int, i, 0, n){ int a; cin >> a; s.insert(a); }
	string out;
	while (q--){
		int t, x; cin >> t >> x;
		if (t == 0) s.insert(x);
		else if (t == 1) s.erase(x);
		else{
			int r;
			if (t == 2) r = x <= (int)s.size() ? *s.find_by_order(x - 1) : -1;
			else if (t == 3) r = s.order_of_key(x + 1);
			else if (t == 4){ int k = s.order_of_key(x + 1); r = k ? *s.find_by_order(k - 1) : -1; }
			else{ auto it = s.lower_bound(x); r = it == s.end() ? -1 : *it; }
			out += to_string(r) + '\n';
		}
	}
	cout << out;
}
