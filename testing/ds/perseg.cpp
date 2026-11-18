#include "../../tex/contents/ds/perseg.hpp"
#include <cassert>

int main(){
	PerSeg s1(vector<PerSeg::Node>(1, {5}));
	assert(s1.get(0, 0, 0).sum == 5);
	int v1 = s1.update(0, 0, {7});
	assert(v1 == 1 && s1.get(1, 0, 0).sum == 7);
	assert(s1.get(0, 0, 0).sum == 5);
	for(int n: {5, 7}){
		vector<PerSeg::Node> a(n);
		vector<vector<ll>> ver(1, vector<ll>(n));
		FOR(i, 0, n - 1)
			a[i] = {i + 1}, ver[0][i] = i + 1;
		PerSeg s(a);
		assert(s.n == n && sz(s.node) == 2 * n - 1);
		FOR(i, 0, n - 1){
			int id = s.build(i, i, a);
			assert(s.lc[id] == 0 && s.rc[id] == 0);
		}
		assert(s.get(0, 0, -1).sum == 0);
		assert(s.get(0, n, n - 1).sum == 0);
		assert(s.get(0, 0, 4).sum == 15);
		assert(s.combine({}, {9}).sum == 9);
		auto oldNode = s.node;
		auto oldLc = s.lc, oldRc = s.rc, oldRoot = s.root;
		int x = s.update(0, 2, {10});
		int y = s.update(0, 4, {-1});
		int z = s.update(x, 0, {0});
		assert(x == 1 && y == 2 && z == 3);
		ver.push_back(ver[0]); ver[1][2] = 10;
		ver.push_back(ver[0]); ver[2][4] = -1;
		ver.push_back(ver[x]); ver[3][0] = 0;
		FOR(i, 0, sz(oldNode) - 1){
			assert(s.node[i].sum == oldNode[i].sum);
			assert(s.lc[i] == oldLc[i]);
			assert(s.rc[i] == oldRc[i]);
		}
		assert(s.root[0] == oldRoot[0]);
		int cnt = sz(s.node);
		FOR(v, 0, sz(ver) - 1)
		FOR(l, 0, n){
			ll sum = 0;
			assert(s.get(v, l, l - 1).sum == 0);
			FOR(r, l, n - 1){
				sum += ver[v][r];
				assert(s.get(v, l, r).sum == sum);
			}
		}
		assert(sz(s.node) == cnt && sz(s.root) == sz(ver));
		assert(sz(s.lc) == cnt && sz(s.rc) == cnt);
		vector<PerSeg::Node> b = {{2}, {-3}};
		s1 = PerSeg(b);
		assert(s1.n == 2 && sz(s1.root) == 1);
		assert(s1.get(0, 0, 1).sum == -1);
		int v = s1.update(0, 1, {8});
		assert(v == 1 && s1.get(v, 0, 1).sum == 10);
		assert(s1.get(0, 0, 1).sum == -1);
		assert(s.get(0, 0, 4).sum == 15);
		assert(sz(s.node) == cnt && sz(s.root) == 4);
	}
	ll big = 1'500'000'000'000'000'000LL;
	PerSeg large(vector<PerSeg::Node>(2, {big}));
	assert(large.get(0, 0, 1).sum == 2 * big);
	int vBig = large.update(0, 0, {-big});
	assert(large.get(vBig, 0, 1).sum == 0);
	assert(large.get(0, 0, 1).sum == 2 * big);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q;
	cin >> n >> q;
	vector<array<ll, 3>> pts(n);
	vector<ll> xs, ys;
	for(auto &[x, y, w]: pts){
		cin >> x >> y >> w;
		ys.push_back(y);
	}
	sort(all(pts)); sort(all(ys));
	ys.erase(unique(all(ys)), ys.end());
	int m = sz(ys);
	PerSeg s(vector<PerSeg::Node>(m + 1));
	int v = 0;
	for(auto [x, y, w]: pts){
		xs.push_back(x);
		int p = lower_bound(all(ys), y) - ys.begin() + 1;
		v = s.update(v, p, {s.get(v, p, p).sum + w});
		assert(v == sz(xs));
	}
	FOR(i, 1, q){
		ll l, d, r, u;
		cin >> l >> d >> r >> u;
		int kl = lower_bound(all(xs), l) - xs.begin();
		int kr = lower_bound(all(xs), r) - xs.begin();
		int yl = lower_bound(all(ys), d) - ys.begin() + 1;
		int yr = lower_bound(all(ys), u) - ys.begin();
		cout << s.get(kr, yl, yr).sum
			- s.get(kl, yl, yr).sum << '\n';
	}
}
