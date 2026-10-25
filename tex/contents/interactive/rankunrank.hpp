#pragma once
#include "../contest/template.hpp"

template<class T> struct RankUnrank{
	vector<T> f; vector<vector<T>> C; // f[i] = i!, C[i][j] = binom(i, j)
	RankUnrank(int n): f(n+1, T(1)), C(n+1, vector<T>(n+1)){
		EFOR(int, i, 0, n+1){
			C[i][0] = T(1); if (i) f[i] = f[i-1] * T(i);
			FOR(int, j, 1, i) C[i][j] = C[i-1][j-1], C[i][j] += C[i-1][j];
		}
	}
	// k-subset of [0, n): sorted c, k <= n
	T rankSubset(const vector<int> &c, int n){
		T r{}; int k = c.size(), p = 0;
		EFOR(int, i, 0, k){
			EFOR(int, x, p, c[i]) r += C[n-1-x][k-1-i];
			p = c[i] + 1;
		}
		return r;
	}
	vector<int> unrankSubset(T r, int n, int k){
		vector<int> c;
		EFOR(int, x, 0, n) if (k){
			if (r < C[n-1-x][k-1]) c.push_back(x), k--;
			else r -= C[n-1-x][k-1];
		}
		return c;
	}
	// permutation of [0, n)
	T rankPerm(const vector<int> &p){
		T r{}; int n = p.size();
		EFOR(int, i, 0, n){
			int s = 0; EFOR(int, j, i+1, n) s += p[j] < p[i];
			r += T(s) * f[n-1-i];
		}
		return r;
	}
	vector<int> unrankPerm(T r, int n){
		vector<int> a(n), p; iota(a.begin(), a.end(), 0);
		EFOR(int, i, 0, n){
			int d = 0; while (r >= f[n-1-i]) r -= f[n-1-i], d++;
			p.push_back(a[d]); a.erase(a.begin() + d);
		}
		return p;
	}
};
