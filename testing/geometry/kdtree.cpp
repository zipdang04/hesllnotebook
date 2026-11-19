#include "../../tex/contents/geometry/kdtree.hpp"
#include <cassert>

int main(){
	ll C = 1000000000;
	PL p{C, C};
	KdTree kd({p});
	auto [d2, q] = kd.nearest({-C, -C});
	assert(d2 == 8000000000000000000LL && q == p);
	tie(d2, q) = kd.nearest(p);
	assert(d2 == 0 && q == p);
	kd = KdTree({p, p});
	tie(d2, q) = kd.nearest(p);
	assert(d2 == 0 && q == p);
	kd = KdTree(vector<PL>(5, {3, 4}));
	assert(sz(kd.t) == 9);
	for(PL a: vector<PL>{{0, 0}, {3, 4}, {-C, C}}){
		tie(d2, q) = kd.nearest(a);
		assert((q == PL{3, 4}));
		assert(d2 == (a - q).dist2());
	}
	vector<PL> pts2 = {{-5, -7}, {-8, -9}};
	KdTree other(pts2);
	assert((kd.nearest({0, 0}).se == PL{3, 4}));
	assert((other.nearest({0, 0}).se == PL{-5, -7}));
	kd = KdTree(pts2);
	for(PL a: vector<PL>{{0, 0}, {-C, C}, {-8, -9}})
		assert(kd.nearest(a) == other.nearest(a));
	FOR(n, 1, 10){
		vector<PL> pts, leaves;
		FOR(i, 0, n - 1)
			pts.push_back({i % 3, i / 3});
		KdTree tree(pts);
		assert(sz(tree.t) == 2 * n - 1);
		vector<int> seen(sz(tree.t)); seen[0] = 1;
		FOR(id, 0, sz(tree.t) - 1){
			auto nd = tree.t[id];
			assert(seen[id] == 1);
			assert(tree.boxDist(id, {nd.x0, nd.y0}) == 0);
			if(!nd.l){
				assert(nd.r == 0);
				assert(nd.x0 == nd.pt.x && nd.x1 == nd.pt.x);
				assert(nd.y0 == nd.pt.y && nd.y1 == nd.pt.y);
				leaves.push_back(nd.pt);
				continue;
			}
			for(int child: {nd.l, nd.r}){
				assert(child > id && child < sz(tree.t));
				seen[child]++;
			}
			auto l = tree.t[nd.l], r = tree.t[nd.r];
			assert(nd.x0 == min(l.x0, r.x0));
			assert(nd.x1 == max(l.x1, r.x1));
			assert(nd.y0 == min(l.y0, r.y0));
			assert(nd.y1 == max(l.y1, r.y1));
		}
		sort(all(pts)); sort(all(leaves));
		assert(pts == leaves);
	}
	mt19937_64 rng(712367);
	for(ll bound: {3LL, C}){
		auto coord = [&](){
			return uniform_int_distribution<ll>(-bound, bound)
				(rng);
		};
		FOR(it, 1, 2000){
			vector<PL> pts(1 + rng() % 40);
			for(PL &a: pts)
				a = {coord(), coord()};
			KdTree tree(pts);
			FOR(j, 0, 19){
				PL a = j < 10 ? pts[rng() % sz(pts)]
					: PL{coord(), coord()};
				ll best = LLONG_MAX;
				for(PL b: pts)
					minimize(best, (a - b).dist2());
				tie(d2, q) = tree.nearest(a);
				assert(d2 == best && (q - a).dist2() == d2);
				assert(find(all(pts), q) != pts.end());
			}
		}
	}
	return 0;
}
