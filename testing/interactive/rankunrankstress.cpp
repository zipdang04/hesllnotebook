#include "../../tex/contents/math/bigint.hpp"
#include "../../tex/contents/interactive/rankunrank.hpp"

template<class T> bool same(T a, ll b){ return a == T(b); }

// Multiset mapping helpers (stars and bars: x_i -> x_i + i, sorted)
template<class T> T rankMulti(RankUnrank<T> &R, vector<int> m, int n){
	EFOR(int, i, 0, (int)m.size()) m[i] += i;
	return R.rankSubset(m, n + (int)m.size() - 1);
}
template<class T> vector<int> unrankMulti(RankUnrank<T> &R, T r, int n, int k){
	vector<int> m = R.unrankSubset(r, n + k - 1, k);
	EFOR(int, i, 0, k) m[i] -= i;
	return m;
}

template<class T> void run(){
	RankUnrank<T> R(24);
	// exhaustive: permutations n<=7 in std::next_permutation order
	FOR(int, n, 0, 7){
		vector<int> p(n); iota(p.begin(), p.end(), 0); ll i = 0;
		do{
			assert(same(R.rankPerm(p), i)); assert(R.unrankPerm(T(i), n) == p); i++;
		}while (next_permutation(p.begin(), p.end()));
	}
	// subsets: all masks of n<=12 enumerated lexicographically as vectors
	FOR(int, n, 0, 12) FOR(int, k, 0, n){
		vector<vector<int>> all;
		vector<int> cur;
		function<void(int)> go = [&](int x){
			if ((int)cur.size() == k){ all.push_back(cur); return; }
			EFOR(int, y, x, n){ cur.push_back(y); go(y+1); cur.pop_back(); }
		};
		go(0); // lexicographic order by construction
		EFOR(int, i, 0, all.size()){
			assert(same(R.rankSubset(all[i], n), i)); assert(R.unrankSubset(T(i), n, k) == all[i]);
		}
	}
	// multisets: n<=6 types, k<=6, non-decreasing sequences in lexicographic order
	FOR(int, n, 1, 6) FOR(int, k, 0, 6){
		vector<vector<int>> all; vector<int> cur;
		function<void(int)> go = [&](int x){
			if ((int)cur.size() == k){ all.push_back(cur); return; }
			EFOR(int, y, x, n){ cur.push_back(y); go(y); cur.pop_back(); }
		};
		go(0);
		EFOR(int, i, 0, all.size()){
			assert(same(rankMulti(R, all[i], n), i)); assert(unrankMulti(R, T(i), n, k) == all[i]);
		}
	}
}

int main(){
	run<ll>(); run<bigint>();

	// ll limit assert block (20! fits in ll; 21! overflows)
	{
		RankUnrank<ll> L(20);
		vector<int> q(20); iota(q.begin(), q.end(), 0); reverse(q.begin(), q.end());
		assert(L.rankPerm(q) == 2432902008176640000LL - 1);
		assert(L.unrankPerm(2432902008176640000LL - 1, 20) == q);
		assert(L.C[20][10] == 184756);
	}

	// big cases (bigint only): perms of 30, C(100,50), multiset
	RankUnrank<bigint> B(120);
	mt19937 rng(1);
	EFOR(int, it, 0, 200){
		int n = 30; vector<int> p(n); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), rng);
		bigint r = B.rankPerm(p); assert(B.unrankPerm(r, n) == p);
		assert(r < B.f[n]);
		vector<int> c; EFOR(int, x, 0, 100) if (rng()%2) c.push_back(x);
		int k = c.size(), nn = 100;
		bigint s = B.rankSubset(c, nn); assert(B.unrankSubset(s, nn, k) == c); assert(s < B.C[nn][k]);
		vector<int> m(20); for (int &v: m) v = rng()%100; sort(m.begin(), m.end());
		bigint q = rankMulti(B, m, 100); assert(unrankMulti(B, q, 100, 20) == m);
	}
	// extremes: first and last ranks
	vector<int> id(30); iota(id.begin(), id.end(), 0);
	assert(B.rankPerm(id) == bigint(0));
	reverse(id.begin(), id.end());
	assert(B.rankPerm(id) + bigint(1) == B.f[30]);
	vector<int> c50(50); iota(c50.begin(), c50.end(), 50);
	assert(B.rankSubset(c50, 100) + bigint(1) == B.C[100][50]);
	assert(B.C[100][50] == bigint("100891344545564193334812497256"));
	assert(B.f[30] == bigint("265252859812191058636308480000000"));
	puts("OK");
}
