#include "../../tex/contents/flow/hungarian.hpp"
#include <cassert>

int main(){
	{
		auto [cost, match] = hungarian(
			vector<vector<ll>>(1, vector<ll>(1)));
		assert(cost == 0 && match == vector<int>({0}));
	}
	{
		vector<vector<ll>> a(2, vector<ll>(2));
		a[1][1] = -7;
		auto [cost, match] = hungarian(a);
		assert(cost == -7 && match == vector<int>({0, 1}));
	}
	{
		vector<vector<ll>> a = {
			{0, 0, 0, 0}, {0, 5, 1, 9}, {0, 1, 2, 9}};
		auto [cost, match] = hungarian(a);
		assert(cost == 2);
		assert(match == vector<int>({0, 2, 1}));
		a[0][2] = a[1][0] = -100;
		assert(hungarian(a) == make_pair(cost, match));
	}
	{
		ll forbidden = 1000000000000LL;
		vector<vector<ll>> a = {{0, 0, 0},
			{0, forbidden, 4}, {0, 3, forbidden}};
		auto [cost, match] = hungarian(a);
		assert(cost == 7 && match == vector<int>({0, 2, 1}));
		FOR(i, 1, 2) assert(a[i][match[i]] != forbidden);
		a = {{0, 0}, {0, forbidden}};
		tie(cost, match) = hungarian(a);
		assert(cost == forbidden);
		assert(a[1][match[1]] == forbidden);
		a = {{0, 0, 0}, {0, forbidden, forbidden}, {0, -1, 0}};
		tie(cost, match) = hungarian(a);
		assert(cost == forbidden - 1);
		assert(a[1][match[1]] == forbidden);
	}
	{
		vector<vector<ll>> a = {
			{0, 0, 0, 0}, {0, 5, 1, 9}, {0, 1, 2, 9}};
		ll best = -oo;
		FOR(j, 1, 3) FOR(k, 1, 3) if(j != k)
			maximize(best, a[1][j] + a[2][k]);
		FOR(i, 1, 2) FOR(j, 1, 3) a[i][j] = -a[i][j];
		auto [cost, match] = hungarian(a);
		assert(-cost == best);
	}
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n; cin >> n;
	vector<vector<ll>> a(n + 1, vector<ll>(n + 1));
	FOR(i, 0, n - 1) FOR(j, 0, n - 1)
		cin >> a[i+1][j+1];
	auto [cost, match] = hungarian(a);
	assert(sz(match) == n + 1 && match[0] == 0);
	vector<char> used(n + 1);
	ll sum = 0;
	FOR(i, 1, n){
		assert(1 <= match[i] && match[i] <= n);
		assert(!used[match[i]]);
		used[match[i]] = 1; sum += a[i][match[i]];
	}
	assert(sum == cost);
	cout << cost << '\n';
	FOR(i, 1, n)
		cout << match[i] - 1 << (i == n ? '\n' : ' ');
}
