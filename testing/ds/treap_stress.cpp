#include "../../tex/contents/ds/treap.hpp"
#include <cassert>

int main(){
	mt19937 rng(123456789);
	const ll mod = 998244353;
	FOR(round, 1, 3000){
		Treap tr;
		vector<ll> a;
		int steps = rng() % 60 + 1;
		FOR(step, 1, steps){
			int n = sz(a), op = rng() % 5;
			if(!n) op = 0;
			if(op == 0){
				int pos = rng() % (n + 1);
				ll x = rng() % mod;
				tr.insert(pos + 1, x);
				a.insert(a.begin() + pos, x);
			}else if(op == 1){
				int pos = rng() % n;
				tr.erase(pos + 1);
				a.erase(a.begin() + pos);
			}else{
				int l = rng() % n, r = rng() % n;
				if(l > r) swap(l, r);
				if(rng() % 4 == 0) l = 0;
				if(rng() % 4 == 0) r = n - 1;
				if(op == 2){
					tr.reverse(l + 1, r + 1);
					reverse(a.begin() + l, a.begin() + r + 1);
				}else if(op == 3){
					ll b = rng() % mod, c = rng() % mod;
					if(rng() % 2) b = rng() % 3;
					tr.update(l + 1, r + 1, {b, c});
					FOR(i, l, r) a[i] = (b * a[i] + c) % mod;
				}else{
					ll sum = 0;
					FOR(i, l, r) sum = (sum + a[i]) % mod;
					auto got = tr.get(l + 1, r + 1);
					assert(got.size == r - l + 1);
					assert(got.sum == sum);
				}
			}
			assert(tr.t[tr.root].size == sz(a));
			ll sum = 0;
			FOR(i, 0, sz(a) - 1){
				assert(tr.get(i + 1, i + 1).sum == a[i]);
				sum = (sum + a[i]) % mod;
			}
			if(!a.empty())
				assert(tr.get(1, sz(a)).sum == sum);
			auto z = tr.t[0];
			assert(z.val == 0 && z.sum == 0);
			assert(z.size == 0 && z.l == 0 && z.r == 0);
			assert(z.pri == 0 && !z.rev);
			assert(z.tag.b == 1 && z.tag.c == 0);
		}
	}
	cout << "OK" << '\n';
}
