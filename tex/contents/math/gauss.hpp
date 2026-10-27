#pragma once
#include "modint.hpp"

typedef vector<vector<modint>> Mat;
// Gauss-Jordan on a (n rows), pivoting only in columns [0, m); extra columns are carried along.
// Returns pivot column of each pivot row (rank = size); rows 0..rank-1 become RREF. det = det of the
// n x m block (0 if some column has no pivot) -- meaningful when n == m.
vector<int> rref(Mat &a, int m, modint *det = 0){
	int n = a.size(), r = 0; vector<int> piv; modint d = 1;
	EFOR(int, c, 0, m){
		int p = r; while (p < n and a[p][c] == 0) p++;
		if (p == n){ d = 0; continue; }
		if (p != r){ swap(a[p], a[r]); d = -d; }
		d *= a[r][c]; modint iv = modint(1) / a[r][c];
		EFOR(int, j, c, a[r].size()) a[r][j] *= iv;
		EFOR(int, i, 0, n) if (i != r and a[i][c] != 0){
			modint t = a[i][c];
			EFOR(int, j, c, a[r].size()) a[i][j] -= t * a[r][j]; }
		piv.push_back(c); r++; }
	if (det) *det = d;
	return piv; }

// A x = b (A n x m mod p). false if inconsistent; else x = one solution, ker = basis of {x: A x = 0}
bool solve(Mat a, vector<modint> b, vector<modint> &x, Mat &ker){
	int n = a.size(), m = n ? a[0].size() : 0;
	EFOR(int, i, 0, n) a[i].push_back(b[i]);
	auto piv = rref(a, m); int r = piv.size();
	EFOR(int, i, r, n) if (a[i][m] != 0) return 0;
	x.assign(m, 0); ker.clear();
	EFOR(int, i, 0, r) x[piv[i]] = a[i][m];
	vector<bool> isp(m); for (int c: piv) isp[c] = 1;
	EFOR(int, f, 0, m) if (!isp[f]){
		vector<modint> v(m); v[f] = 1;
		EFOR(int, i, 0, r) v[piv[i]] = -a[i][f];
		ker.push_back(v); }
	return 1; }

// a <- a^-1 (n x n); false if singular (a unchanged)
bool inverse(Mat &a){
	int n = a.size(); Mat b = a;
	EFOR(int, i, 0, n){ b[i].resize(2 * n); b[i][n + i] = 1; }
	if ((int)rref(b, n).size() < n) return 0;
	EFOR(int, i, 0, n) a[i].assign(b[i].begin() + n, b[i].end());
	return 1; }

const int MAXB = 4097; // bitset width: needs m + 1 <= MAXB
// A x = b over GF(2). Row i = bits 0..m-1 of a[i] (A) and bit m (b_i). Returns rank, or -1 if inconsistent;
// x = one solution, ker (optional) = kernel basis. O(n^2 m / 64)
int solveXor(vector<bitset<MAXB>> a, int m, bitset<MAXB> &x, vector<bitset<MAXB>> *ker = 0){
	int n = a.size(), r = 0; vector<int> piv;
	EFOR(int, c, 0, m){
		int p = r; while (p < n and !a[p][c]) p++;
		if (p == n) continue;
		swap(a[p], a[r]);
		EFOR(int, i, 0, n) if (i != r and a[i][c]) a[i] ^= a[r];
		piv.push_back(c); r++; }
	EFOR(int, i, r, n) if (a[i][m]) return -1;
	x.reset(); EFOR(int, i, 0, r) x[piv[i]] = a[i][m];
	if (ker){
		ker->clear(); vector<bool> isp(m); for (int c: piv) isp[c] = 1;
		EFOR(int, f, 0, m) if (!isp[f]){
			bitset<MAXB> v; v[f] = 1;
			EFOR(int, i, 0, r) v[piv[i]] = a[i][f];
			ker->push_back(v); } }
	return r; }
