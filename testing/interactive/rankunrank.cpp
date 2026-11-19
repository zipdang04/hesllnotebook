#include "../../tex/contents/interactive/rankunrank.hpp"
#include <cassert>

int main(){
	ll pascal[40][40] = {};
	FOR(n, 0, 39){
		pascal[n][0] = pascal[n][n] = 1;
		FOR(k, 1, n - 1)
			pascal[n][k] = pascal[n-1][k-1] + pascal[n-1][k];
		FOR(k, -2, n + 2){
			ll want = k < 0 || k > n ? 0 : pascal[n][k];
			assert(rankBinom(n, k) == Bigint(want));
		}
	}
	assert(rankBinom(100, 50).str()
		== "100891344545564193334812497256");
	auto colex = [](const vector<int> &a,
			const vector<int> &b){
		return lexicographical_compare(a.rbegin(), a.rend(),
			b.rbegin(), b.rend());
	};
	FOR(n, 0, 9)
		FOR(k, 0, n){
			vector<vector<int>> subsets;
			FOR(mask, 0, (1 << n) - 1) if(cntBit(mask) == k){
				vector<int> a;
				FOR(x, 1, n) if(testBit(mask, x - 1))
					a.push_back(x);
				subsets.push_back(a);
			}
			sort(all(subsets), colex);
			assert(rankBinom(n, k) == Bigint(sz(subsets)));
			FOR(i, 0, sz(subsets) - 1){
				assert(rankSubset(subsets[i]) == Bigint(i));
				assert(unrankSubset(n, k, Bigint(i)) == subsets[i]);
			}
		}
	vector<vector<int>> subsets = {
		{1, 2}, {1, 3}, {2, 3}, {1, 4}, {2, 4}, {3, 4}};
	FOR(i, 0, sz(subsets) - 1){
		vector<int> copy = subsets[i];
		assert(rankSubset(subsets[i]) == Bigint(i));
		assert(subsets[i] == copy);
		assert(unrankSubset(4, 2, Bigint(i)) == copy);
	}
	FOR(n, 1, 5)
		FOR(k, 0, 6){
			vector<vector<int>> multisets(1);
			FOR(len, 1, k){
				vector<vector<int>> next;
				for(const auto &a: multisets){
					int lo = a.empty() ? 1 : a.back();
					FOR(x, lo, n){
						vector<int> b = a;
						b.push_back(x); next.push_back(b);
					}
				}
				multisets = move(next);
			}
			sort(all(multisets), colex);
			assert(rankBinom(n + k - 1, k)
				== Bigint(sz(multisets)));
			FOR(i, 0, sz(multisets) - 1){
				assert(rankMultiset(multisets[i]) == Bigint(i));
				assert(unrankMultiset(n, k, Bigint(i))
					== multisets[i]);
			}
		}
	vector<vector<int>> multisets = {
		{1, 1}, {1, 2}, {2, 2}, {1, 3}, {2, 3}, {3, 3}};
	FOR(i, 0, sz(multisets) - 1){
		vector<int> copy = multisets[i];
		assert(rankMultiset(multisets[i]) == Bigint(i));
		assert(multisets[i] == copy);
		assert(unrankMultiset(3, 2, Bigint(i)) == copy);
	}
	FOR(n, 0, 8){
		vector<int> p(n);
		iota(all(p), 1);
		int i = 0;
		do{
			assert(rankPerm(p) == Bigint(i));
			assert(unrankPerm(n, Bigint(i)) == p);
			i++;
		}while(next_permutation(all(p)));
	}
	Bigint fact = 1;
	FOR(i, 1, 25)
		fact = fact * i;
	vector<int> p(25);
	iota(all(p), 1); reverse(all(p));
	vector<int> copy = p;
	assert(rankPerm(p) == fact - 1);
	assert(p == copy);
	assert(unrankPerm(25, fact - 1) == p);
	Bigint count = rankBinom(100, 50);
	vector<int> first(50), last(50);
	iota(all(first), 1); iota(all(last), 51);
	assert(unrankSubset(100, 50, 0) == first);
	assert(unrankSubset(100, 50, count - 1) == last);
	assert(rankSubset(first) == Bigint(0));
	assert(rankSubset(last) == count - 1);
	count = rankBinom(3000, 1500);
	first.resize(1500); last.resize(1500);
	iota(all(first), 1); iota(all(last), 1501);
	assert(rankSubset(first) == Bigint(0));
	assert(rankSubset(last) == count - 1);
	vector<int> mid = unrankSubset(3000, 1500, count / 2);
	assert(rankSubset(mid) == count / 2);
	assert(rankMultiset(vector<int>(1500, 1)) == Bigint(0));
	assert(rankMultiset(vector<int>(1500, 1501))
		== count - 1);
	assert(rankSubset({INT_MAX}) == Bigint(INT_MAX) - 1);
	assert(unrankMultiset(INT_MAX, 1, Bigint(INT_MAX) - 1)
		== vector<int>{INT_MAX});
}
