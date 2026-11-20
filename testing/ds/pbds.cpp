#include "../../tex/contents/ds/pbds.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	{
		OrderedSet<int> e, t;
		assert(e.find_by_order(e.size()) == e.end());
		for(int x: {10, 20, 30}) t.insert(x);
		assert(t.find_by_order(t.size()) == t.end());
		assert(t.order_of_key(5) == 0);
		assert(t.order_of_key(99) == 3);
		assert(t.order_of_key(20) == 1);
		assert(*t.find_by_order(2) == 30);
		// two sets are independent; reassignment empties
		e.insert(7);
		assert(sz(e) == 1 && sz(t) == 3);
		t = OrderedSet<int>();
		assert(t.empty() && sz(e) == 1);
		OrderedSet<pii> ms;
		ms.insert({3, 0}), ms.insert({3, 1});
		ms.insert({5, 2});
		assert(sz(ms) == 3);
		assert(ms.order_of_key({3, INT_MAX}) == 2);
		assert(ms.find_by_order(1)->se == 1);
		OrderedMap<int, int> m;
		m[30] = 3, m[10] = 1, m[20] = 2, m[10] += 5;
		assert(sz(m) == 3 && m.order_of_key(25) == 2);
		auto it = m.find_by_order(0);
		assert(it->fi == 10 && it->se == 6);
		m.erase(10);
		assert(m.find_by_order(0)->se == 2);
	}
	OrderedSet<int> s;
	int n, q;
	cin >> n >> q;
	FOR(i, 1, n){
		int a;
		cin >> a;
		s.insert(a);
	}
	FOR(i, 1, q){
		int t, x;
		cin >> t >> x;
		if(t == 0) s.insert(x);
		else if(t == 1) s.erase(x);
		else if(t == 2){
			auto it = s.find_by_order(x - 1);
			cout << (it == s.end() ? -1 : *it) << '\n';
		}else if(t == 3) cout << s.order_of_key(x + 1) << '\n';
		else if(t == 4){
			int k = s.order_of_key(x + 1);
			cout << (k ? *s.find_by_order(k - 1) : -1) << '\n';
		}else{
			auto it = s.find_by_order(s.order_of_key(x));
			cout << (it == s.end() ? -1 : *it) << '\n';
		}
	}
}
