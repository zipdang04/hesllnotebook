#include "../../tex/contents/math/bigint.hpp"
#include "../../tex/contents/interactive/rankunrank.hpp"

template<class T> bool same(T a, ll b){ return a == T(b); }

// Multiset mapping helpers (stars and bars: x_i -> x_i + i, sorted)
template<class T> T rankMulti(RankUnrank<T> &R, vector<int> m, int n){
	FOR(int, i, 0, (int)m.size() - 1) m[i] += i;
	return R.rankSubset(m, n + (int)m.size() - 1);
}
template<class T> vector<int> unrankMulti(RankUnrank<T> &R, T r, int n, int k){
	vector<int> m = R.unrankSubset(r, n + k - 1, k);
	FOR(int, i, 0, k - 1) m[i] -= i;
	return m;
}

template<class T> void run(){
	// ll: subsets fit up to n = 66, permutations up to n = 20
	RankUnrank<T> R(20);
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
			FOR(int, y, x, n - 1){ cur.push_back(y); go(y+1); cur.pop_back(); }
		};
		go(0); // lexicographic order by construction
		FOR(int, i, 0, (int)all.size() - 1){
			assert(same(R.rankSubset(all[i], n), i)); assert(R.unrankSubset(T(i), n, k) == all[i]);
		}
	}
	// multisets: n<=6 types, k<=6, non-decreasing sequences in lexicographic order
	FOR(int, n, 1, 6) FOR(int, k, 0, 6){
		vector<vector<int>> all; vector<int> cur;
		function<void(int)> go = [&](int x){
			if ((int)cur.size() == k){ all.push_back(cur); return; }
			FOR(int, y, x, n - 1){ cur.push_back(y); go(y); cur.pop_back(); }
		};
		go(0);
		FOR(int, i, 0, (int)all.size() - 1){
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
	// ll subsets up to n = 66: C(66, 33) fits
	{
		RankUnrank<ll> L(66);
		vector<int> c(33); iota(c.begin(), c.end(), 33);
		ll last = L.C[66][33] - 1;
		assert(L.rankSubset(c, 66) == last);
		assert(L.unrankSubset(last, 66, 33) == c);
	}
	// permutations need no table: n = 0 works, n = 1000 is fast
	{
		RankUnrank<bigint> Z(0);
		mt19937 rg(7);
		vector<int> p(1000); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), rg);
		assert(Z.unrankPerm(Z.rankPerm(p), 1000) == p);
		RankUnrank<ll> Y(0);
		vector<int> q(20); iota(q.begin(), q.end(), 0); shuffle(q.begin(), q.end(), rg);
		assert(Y.unrankPerm(Y.rankPerm(q), 20) == q);
	}

	// big cases (bigint only): perms of 30, C(100,50), multiset
	RankUnrank<bigint> B(120);
	auto fact = [](int n){ bigint f(1); FOR(int, i, 1, n) f = f * bigint(i); return f; };
	mt19937 rng(1);
	FOR(int, it, 0, 199){
		int n = 30; vector<int> p(n); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), rng);
		bigint r = B.rankPerm(p); assert(B.unrankPerm(r, n) == p);
		assert(r < fact(n));
		vector<int> c; FOR(int, x, 0, 99) if (rng()%2) c.push_back(x);
		int k = c.size(), nn = 100;
		bigint s = B.rankSubset(c, nn); assert(B.unrankSubset(s, nn, k) == c); assert(s < B.C[nn][k]);
		vector<int> m(20); for (int &v: m) v = rng()%100; sort(m.begin(), m.end());
		bigint q = rankMulti(B, m, 100); assert(unrankMulti(B, q, 100, 20) == m);
	}
	// extremes: first and last ranks
	vector<int> id(30); iota(id.begin(), id.end(), 0);
	assert(B.rankPerm(id) == bigint(0));
	reverse(id.begin(), id.end());
	assert(B.rankPerm(id) + bigint(1) == fact(30));
	vector<int> c50(50); iota(c50.begin(), c50.end(), 50);
	assert(B.rankSubset(c50, 100) + bigint(1) == B.C[100][50]);
	assert(B.C[100][50] == bigint("100891344545564193334812497256"));
	assert(fact(30) == bigint("265252859812191058636308480000000"));
	puts("OK");
}
