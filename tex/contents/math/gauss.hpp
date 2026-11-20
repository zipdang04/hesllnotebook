#pragma once
#include "modint.hpp"

// Gauss-Jordan over Mint<MOD>, MOD prime, 0-based a[i][j].
// gauss: O(n m min(n, c)). solveXor: GF(2), bitset rows,
// O(n m min(n, m) / 64).
// gauss: a -> reduced row echelon form on columns 0..c-1
template<int MOD>
pair<int, Mint<MOD>> gauss(vector<vector<Mint<MOD>>> &a,
		int c){
	int n = sz(a), rank = 0;
	Mint<MOD> det = 1;
	for(int col = 0; col < c && rank < n; col++){
		int r = rank;
		while(r < n && a[r][col] == 0) r++; if(r == n) continue;
		if(r != rank) swap(a[r], a[rank]), det = -det;
		det *= a[rank][col];
		Mint<MOD> iv = a[rank][col].inv();
		int m = sz(a[rank]);
		FOR(j, col, m - 1) a[rank][j] *= iv;
		FOR(i, 0, n - 1) if(i != rank && a[i][col] != 0){
			Mint<MOD> f = a[i][col];
			FOR(j, col, m - 1) a[i][j] -= f * a[rank][j];
		}
		rank++;
	}
	if(rank < c) det = 0;
	return {rank, det};
}
// a is n x m, n >= 1; {} if no solution, else s[0] = x,
// s[1..] = kernel basis
template<int MOD>
vector<vector<Mint<MOD>>> solveLinear(
		vector<vector<Mint<MOD>>> a,
		const vector<Mint<MOD>> &b){
	int n = sz(a), m = sz(a[0]);
	FOR(i, 0, n - 1) a[i].push_back(b[i]);
	int rank = gauss(a, m).fi;
	FOR(r, rank, n - 1) if(a[r][m] != 0) return {};
	vector<int> piv(rank); vector<char> isPiv(m);
	FOR(r, 0, rank - 1){
		while(a[r][piv[r]] == 0) piv[r]++; isPiv[piv[r]] = 1;
	}
	vector<vector<Mint<MOD>>> s(1,
		vector<Mint<MOD>>(m));
	FOR(r, 0, rank - 1) s[0][piv[r]] = a[r][m];
	FOR(f, 0, m - 1) if(!isPiv[f]){
		vector<Mint<MOD>> v(m); v[f] = 1;
		FOR(r, 0, rank - 1) v[piv[r]] = -a[r][f];
		s.push_back(v);
	}
	return s;
}
template<int MOD>
bool matInverse(vector<vector<Mint<MOD>>> &a){
	int n = sz(a);
	FOR(i, 0, n - 1) a[i].resize(2 * n), a[i][n + i] = 1;
	if(gauss(a, n).fi < n) return false;
	FOR(i, 0, n - 1)
		a[i].erase(a[i].begin(), a[i].begin() + n);
	return true;
}
using BitRow = bitset<4'097>;
vector<BitRow> solveXor(vector<BitRow> a,
		const vector<int> &b, int m){
	int n = sz(a), rank = 0;
	FOR(i, 0, n - 1) a[i][m] = b[i];
	for(int col = 0; col < m && rank < n; col++){
		int r = rank;
		while(r < n && !a[r][col]) r++; if(r == n) continue;
		swap(a[r], a[rank]);
		FOR(i, 0, n - 1) if(i != rank && a[i][col])
			a[i] ^= a[rank];
		rank++;
	}
	FOR(r, rank, n - 1) if(a[r][m]) return {};
	vector<int> piv(rank); BitRow isPiv;
	FOR(r, 0, rank - 1){
		while(!a[r][piv[r]]) piv[r]++; isPiv[piv[r]] = 1;
	}
	vector<BitRow> s(1);
	FOR(r, 0, rank - 1) s[0][piv[r]] = a[r][m];
	FOR(f, 0, m - 1) if(!isPiv[f]){
		BitRow v; v[f] = 1;
		FOR(r, 0, rank - 1) v[piv[r]] = a[r][f];
		s.push_back(v);
	}
	return s;
}
