#include "../../tex/contents/ds/mo.hpp"
#include <cassert>

int m;
vector<int> a, bit;
ll cur, cnt;

int prefix(int x){
	int s = 0;
	for(; x > 0; x -= x & -x) s += bit[x];
	return s;
}
void put(int x, int d){
	cnt += d;
	for(; x <= m; x += x & -x) bit[x] += d;
}
// window elements greater (right) or smaller than a[i]
ll cross(int i, bool right){
	return right ? cnt - prefix(a[i]) : prefix(a[i] - 1);
}
void add(int i, int right){
	cur += cross(i, right);
	put(a[i], 1);
}
void del(int i, int right){
	put(a[i], -1);
	cur -= cross(i, right);
}

// runs moQueries on b[1..], asserting the window protocol
vector<ll> check(const vector<int> &b,
		const vector<pair<int, int>> &q){
	a = b, m = 10;
	bit.assign(m + 1, 0), cur = cnt = 0;
	int L = 1, R = 0;
	auto addP = [&](int i, int right){
		assert(right ? i == R + 1 : i == L - 1);
		right ? R++ : L--;
		add(i, right);
	};
	auto delP = [&](int i, int right){
		assert(R >= L && i == (right ? R : L));
		right ? R-- : L++;
		del(i, right);
	};
	return moQueries(q, addP, delP, [&]{ return cur; });
}

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	vector<int> b = {0, 3, 1, 2, 3, 1};
	assert(check(b, {}).empty());
	int calls = 0;
	auto move = [&](int, int){ calls++; };
	auto get = [&]{ calls++; return 0; };
	assert(moQueries({}, move, move, get).empty());
	assert(calls == 0);
	vector<pair<int, int>> qs;
	FOR(l, 1, 5)
		FOR(r, l, 5) qs.push_back({l, r});
	qs.push_back({2, 4}), qs.push_back({2, 4});
	vector<ll> res = check(b, qs);
	assert(sz(res) == sz(qs));
	FOR(i, 0, sz(qs) - 1){
		ll inv = 0;
		auto [l, r] = qs[i];
		FOR(x, l, r)
			FOR(y, x + 1, r) inv += b[x] > b[y];
		assert(res[i] == inv);
		if(l == r) assert(res[i] == 0);
	}
	assert(res[sz(qs) - 1] == res[sz(qs) - 2]);
	int n, nq;
	cin >> n >> nq;
	vector<int> v(n);
	for(int &x: v) cin >> x;
	vector<int> xs = v;
	sort(all(xs));
	xs.erase(unique(all(xs)), xs.end());
	a.assign(n + 1, 0);
	FOR(i, 1, n)
		a[i] = lower_bound(all(xs), v[i - 1]) - xs.begin() + 1;
	m = sz(xs), bit.assign(m + 1, 0), cur = cnt = 0;
	vector<pair<int, int>> q(nq);
	for(auto &[l, r]: q){
		cin >> l >> r;
		l++;
	}
	for(ll x: moQueries(q, add, del, [&]{ return cur; }))
		cout << x << '\n';
}
