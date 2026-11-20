#include "../../tex/contents/ds/lichao.hpp"
#include <cassert>

// Notebook recipe: for a maximum add (-a, -b) and negate get().
int main(){
	mt19937_64 rng(7);
	auto rnd = [&](ll lo, ll hi){ return lo + (ll)(rng() % (hi - lo + 1)); };
	FOR(it, 1, 3000){
		int k = rnd(1, 30); ll C = it % 3 ? 50 : 1000000000;
		vector<ll> pts(k); for(ll &x: pts) x = rnd(-C, C);
		LiChao lc(pts);
		vector<array<ll, 4>> lines; // a, b, l, r
		FOR(q, 1, 60){
			ll x = pts[rng() % k];
			if(rng() % 2){
				ll a = rnd(-C, C), b = rnd(-C * C, C * C);
				ll l = rnd(-C, C), r = rnd(-C, C);
				if(rng() % 2) l = LLONG_MIN, r = LLONG_MAX, lc.addLine(-a, -b);
				else{ if(l > r) swap(l, r); lc.addSegment(-a, -b, l, r); }
				lines.push_back({a, b, l, r});
			}
			ll best = -LiChao::oo;
			for(auto [a, b, l, r]: lines) if(l <= x && x <= r) best = max(best, a * x + b);
			assert(-lc.get(x) == best);
		}
	}
	puts("OK");
}
