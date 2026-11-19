#include "../../tex/contents/interactive/rankunrank.hpp"
#include <cassert>

struct Oracle{
	vector<vector<Bigint>> binom;
	vector<Bigint> fact;
	Oracle(): binom(201, vector<Bigint>(201)), fact(61){
		FOR(n, 0, 200){
			binom[n][0] = 1;
			FOR(k, 1, n)
				binom[n][k] = binom[n-1][k-1]
					+ binom[n-1][k];
		}
		fact[0] = 1;
		FOR(n, 1, 60) fact[n] = fact[n-1] * n;
	}
	Bigint subsetRank(const vector<int> &a) const{
		Bigint rank = 0;
		FOR(i, 0, sz(a) - 1)
			rank += binom[a[i]-1][i+1];
		return rank;
	}
	vector<int> subsetUnrank(int n, int k,
			Bigint rank) const{
		vector<int> a(k);
		int x = n - 1;
		REV(j, k, 1){
			while(binom[x][j] > rank) x--;
			a[j-1] = x + 1;
			rank -= binom[x][j];
			x--;
		}
		return a;
	}
	Bigint permRank(const vector<int> &a) const{
		Bigint rank = 0;
		FOR(i, 0, sz(a) - 1){
			int d = 0;
			FOR(j, i + 1, sz(a) - 1)
				d += a[j] < a[i];
			rank += fact[sz(a)-1-i] * d;
		}
		return rank;
	}
	vector<int> permUnrank(int n, Bigint rank) const{
		vector<int> unused(n), a;
		iota(all(unused), 1);
		REV(j, n - 1, 0){
			int d = 0;
			while(!(fact[j] > rank)){
				rank -= fact[j];
				d++;
			}
			a.push_back(unused[d]);
			unused.erase(unused.begin() + d);
		}
		return a;
	}
};
void checkChoice(int n, const vector<int> &a,
		bool multi, const Oracle &brute){
	int k = sz(a), universe = n;
	vector<int> b = a, copy = a;
	if(multi){
		universe += k - 1;
		FOR(i, 0, k - 1) b[i] += i;
	}
	Bigint count = brute.binom[universe][k];
	Bigint rank = brute.subsetRank(b);
	assert(rankBinom(universe, k) == count);
	if(multi) assert(rankMultiset(a) == rank);
	else assert(rankSubset(a) == rank);
	assert(a == copy);
	vector<Bigint> ranks = {rank, 0, 1, count - 1,
		count / 2};
	for(Bigint r: ranks){
		if(!(count > r)) continue;
		vector<int> want = brute.subsetUnrank(
			universe, k, r);
		if(multi)
			FOR(i, 0, k - 1)
				want[i] -= i;
		if(r == rank) assert(want == a);
		if(multi){
			assert(unrankMultiset(n, k, r) == want);
			assert(rankMultiset(want) == r);
		}else{
			assert(unrankSubset(n, k, r) == want);
			assert(rankSubset(want) == r);
		}
	}
}
void checkPerm(const vector<int> &a, const Oracle &brute){
	int n = sz(a);
	vector<int> copy = a;
	Bigint rank = brute.permRank(a), count = brute.fact[n];
	assert(rankPerm(a) == rank);
	assert(a == copy);
	vector<Bigint> ranks = {rank, 0, 1, count - 1,
		count / 2};
	for(Bigint r: ranks){
		if(!(count > r)) continue;
		vector<int> want = brute.permUnrank(n, r);
		if(r == rank) assert(want == a);
		assert(unrankPerm(n, r) == want);
		assert(rankPerm(want) == r);
	}
}
int main(){
	Oracle brute;
	mt19937_64 rng(732);
	checkChoice(0, {}, false, brute);
	checkChoice(1, {}, true, brute);
	checkChoice(1, {1}, false, brute);
	checkChoice(1, vector<int>(100, 1), true, brute);
	checkPerm({}, brute);
	checkPerm({1}, brute);
	FOR(it, 0, 179){
		int type = it % 3;
		int n = rng() % (type == 0 ? 201 : 61);
		if(type == 1) n = 1 + rng() % 100;
		if(it < 3)
			n = type == 0 ? 200 : type == 1 ? 100 : 60;
		int x = rng() % 201;
		int y = (int)(rng() % (x + 5)) - 2;
		Bigint want = 0;
		if(y >= 0 && y <= x) want = brute.binom[x][y];
		assert(rankBinom(x, y) == want);
		if(type == 2){
			vector<int> a(n);
			iota(all(a), 1);
			shuffle(all(a), rng);
			checkPerm(a, brute);
		}else{
			int k = rng() % (type == 0 ? n + 1 : 101);
			if(it < 2) k = 100;
			vector<int> a(type == 0 ? n : k);
			if(type == 0){
				iota(all(a), 1);
				shuffle(all(a), rng);
				a.resize(k);
			}else{
				for(int &v: a) v = 1 + rng() % n;
			}
			sort(all(a));
			checkChoice(n, a, type == 1, brute);
		}
	}
}
