#pragma once
#include "../contest/template.hpp"

// Simplex: max c.x subject to Ax <= b, x >= 0. A is m x n,
// all 0-based. val = -oo: no solution, oo: unbounded.
// O(mn) per pivot, exponential in the worst case.
struct Simplex{
	// ---- EDIT: tolerance ----
	static constexpr double EPS = 1e-8;
	// ---- end of EDIT ----
	double val;
	vector<double> x;
	int m, n;
	vector<int> N, B;
	vector<vector<double>> D;
	Simplex(const vector<vector<double>> &A,
			const vector<double> &b, const vector<double> &c):
			m(sz(b)), n(sz(c)), N(n + 1), B(m),
			D(m + 2, vector<double>(n + 2)){
		FOR(i, 0, m - 1) FOR(j, 0, n - 1)
			D[i][j] = A[i][j];
		FOR(i, 0, m - 1){
			B[i] = n + i; D[i][n] = -1; D[i][n+1] = b[i];
		}
		FOR(j, 0, n - 1){ N[j] = j; D[m][j] = -c[j]; }
		N[n] = -1; D[m+1][n] = 1;
		int r = 0;
		FOR(i, 1, m - 1) if(D[i][n+1] < D[r][n+1])
			r = i;
		if(D[r][n+1] < -EPS){
			pivot(r, n);
			if(!iterate(2) || D[m+1][n+1] < -EPS){
				val = -oo; return;
			}
			FOR(i, 0, m - 1) if(B[i] == -1){
				int s = 0;
				FOR(j, 1, n) if(ltj(D[i], j, s))
					s = j;
				pivot(i, s);
			}
		}
		bool ok = iterate(1); x.assign(n, 0);
		FOR(i, 0, m - 1) if(B[i] < n)
			x[B[i]] = D[i][n+1];
		val = ok ? D[m][n+1] : oo;
	}
	bool ltj(const vector<double> &row, int j, int s){
		return s == -1 || pair(row[j], N[j])
			< pair(row[s], N[s]);
	}
	void pivot(int r, int s){
		double *a = D[r].data(), inv = 1 / a[s];
		FOR(i, 0, m + 1) if(i != r && abs(D[i][s]) > EPS){
			double *b = D[i].data(), inv2 = b[s] * inv;
			FOR(j, 0, n + 1)
				b[j] -= a[j] * inv2;
			b[s] = a[s] * inv2;
		}
		FOR(j, 0, n + 1) if(j != s)
			D[r][j] *= inv;
		FOR(i, 0, m + 1) if(i != r)
			D[i][s] *= -inv;
		D[r][s] = inv; swap(B[r], N[s]);
	}
	bool iterate(int phase){
		int row = m + phase - 1;
		// Bland's rule after a degenerate pivot: no cycling
		bool bland = false;
		while(true){
			int s = -1;
			FOR(j, 0, n) if(N[j] != -phase && (bland
				? D[row][j] < -EPS && (s == -1 || N[j] < N[s])
				: ltj(D[row], j, s)))
				s = j;
			if(s == -1 || D[row][s] >= -EPS) return true;
			int r = -1;
			FOR(i, 0, m - 1){
				if(D[i][s] <= EPS) continue;
				if(r == -1 || pair(D[i][n+1] / D[i][s], B[i])
					< pair(D[r][n+1] / D[r][s], B[r]))
					r = i;
			}
			if(r == -1) return false;
			bland = D[r][n+1] / D[r][s] < EPS;
			pivot(r, s);
		}
	}
};
