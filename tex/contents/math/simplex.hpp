#pragma once
#include "../contest/template.hpp"

// maximize c^T x s.t. Ax <= b, x >= 0 (A: m x n). Two-phase, ties broken by smallest index
template<class T = double> struct Simplex{
	static constexpr T eps = 1e-9, inf = numeric_limits<T>::infinity();
	int m, n; vector<int> N, B; vector<vector<T>> D;
	Simplex(const vector<vector<T>>& A, const vector<T>& b, const vector<T>& c):
			m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, vector<T>(n + 2)){
		EFOR(int, i, 0, m){
			EFOR(int, j, 0, n) D[i][j] = A[i][j];
			B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i];
		}
		EFOR(int, j, 0, n){ N[j] = j; D[m][j] = -c[j]; }
		N[n] = -1; D[m + 1][n] = 1;
	}
	void pivot(int r, int s){
		T inv = 1 / D[r][s];
		EFOR(int, i, 0, m + 2) if (i != r && abs(D[i][s]) > eps){
			T k = D[i][s] * inv;
			EFOR(int, j, 0, n + 2) if (j != s) D[i][j] -= D[r][j] * k;
		}
		EFOR(int, j, 0, n + 2) if (j != s) D[r][j] *= inv;
		EFOR(int, i, 0, m + 2) if (i != r) D[i][s] *= -inv;
		D[r][s] = inv; swap(B[r], N[s]);
	}
	bool _lt(T a, int i, T b, int j){ return make_pair(a, i) < make_pair(b, j); }
	bool simplex(int phase){ // phase 2: row m+1 (artificial), phase 1: row m (objective)
		for (int x = m + phase - 1;;){
			int s = -1, r = -1;
			EFOR(int, j, 0, n + 1) if (N[j] != -phase && (s < 0 || _lt(D[x][j], N[j], D[x][s], N[s]))) s = j;
			if (s < 0 || D[x][s] >= -eps) return true;
			EFOR(int, i, 0, m) if (D[i][s] > eps && (r < 0
					|| _lt(D[i][n + 1] / D[i][s], B[i], D[r][n + 1] / D[r][s], B[r]))) r = i;
			if (r < 0) return false;
			pivot(r, s);
		}
	}
	// -inf: infeasible, inf: unbounded (x = some feasible point), else the max and an optimal x
	T solve(vector<T>& x){
		int r = 0;
		EFOR(int, i, 1, m) if (D[i][n + 1] < D[r][n + 1]) r = i;
		if (D[r][n + 1] < -eps){
			pivot(r, n);
			if (!simplex(2) || D[m + 1][n + 1] < -eps) return -inf;
			EFOR(int, i, 0, m) if (B[i] == -1){ // artificial still basic at 0: pivot it out
				int s = 0;
				EFOR(int, j, 1, n + 1) if (_lt(D[i][j], N[j], D[i][s], N[s])) s = j;
				pivot(i, s);
			}
		}
		bool ok = simplex(1); x.assign(n, 0);
		EFOR(int, i, 0, m) if (B[i] < n) x[B[i]] = D[i][n + 1];
		return ok ? D[m][n + 1] : inf;
	}
};
