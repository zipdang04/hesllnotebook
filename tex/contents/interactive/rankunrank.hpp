#pragma once
#include "../math/bigint.hpp"

// Rank 0 first; labels 1..n, vectors 0-based. Subsets and
// multisets: colex, rank O(k^2), unrank O(n + k) Bigint
// ops. Permutations: lex, O(n log n) + O(n) Bigint ops.
Bigint rankBinom(int n, int k){
	if(k < 0 || k > n)
		return 0;
	minimize(k, n - k);
	Bigint c = 1;
	FOR(i, 1, k)
		c = c * (n - k + i) / i;
	return c;
}
Bigint rankSubset(const vector<int> &a){
	Bigint r = 0;
	FOR(i, 0, sz(a) - 1)
		r += rankBinom(a[i] - 1, i + 1);
	return r;
}
vector<int> unrankSubset(int n, int k, Bigint rank){
	vector<int> a(k);
	Bigint c = rankBinom(n, k);
	int x = n;
	REV(j, k, 1){
		// c = C(x, j): lower x until C(x, j) <= rank
		while(c > rank)
			c = c * (x - j) / x, x--;
		a[j-1] = x + 1; rank -= c;
		// c = C(x - 1, j - 1); x > 0 when j > 1
		if(j > 1)
			c = c * j / x;
		x--;
	}
	return a;
}
Bigint rankMultiset(const vector<int> &a){
	vector<int> b = a;
	FOR(i, 0, sz(b) - 1)
		b[i] += i;
	return rankSubset(b);
}
vector<int> unrankMultiset(int n, int k, Bigint rank){
	vector<int> a = unrankSubset(n - 1 + k, k, rank);
	FOR(i, 0, k - 1)
		a[i] -= i;
	return a;
}
Bigint rankPerm(const vector<int> &a){
	int n = sz(a);
	vector<int> bit(n + 1);
	FOR(i, 1, n)
		bit[i] = i & -i;
	Bigint r = 0;
	FOR(i, 0, n - 1){
		int d = 0;
		for(int x = a[i] - 1; x; x &= x - 1)
			d += bit[x];
		for(int x = a[i]; x <= n; x += x & -x)
			bit[x]--;
		r = r * (n - i) + d;
	}
	return r;
}
vector<int> unrankPerm(int n, Bigint rank){
	vector<int> d(n), bit(n + 1), a(n);
	FOR(j, 1, n){
		auto [q, rem] = rank.divMod(j);
		d[n-j] = stoi(rem.str()); rank = q;
	}
	// Fenwick tree of unused labels: all unused at first
	FOR(i, 1, n)
		bit[i] = i & -i;
	FOR(i, 0, n - 1){
		int x = 0, k = d[i];
		// the (d[i] + 1)-th unused label, by binary lifting
		for(int pw = 1 << log2(n); pw; pw >>= 1)
			if(x + pw <= n && bit[x+pw] <= k)
				x += pw, k -= bit[x];
		a[i] = x + 1;
		for(int y = x + 1; y <= n; y += y & -y)
			bit[y]--;
	}
	return a;
}
