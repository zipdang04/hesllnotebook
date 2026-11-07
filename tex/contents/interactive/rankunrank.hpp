#pragma once
#include "../contest/template.hpp"
template<class T> struct RankUnrank{
	vector<vector<T>> C;
	RankUnrank(int n): C(n + 1, vector<T>(n + 1)){
		FOR(int, i, 0, n){
			C[i][0] = T(1);
			FOR(int, j, 1, i) C[i][j] = C[i-1][j-1] + C[i-1][j];
		}
	}
	T rankSubset(const vector<int> &c, int n){
		T r{}; int k = c.size(), p = 0;
		FOR(int, i, 0, k - 1){
			FOR(int, x, p, c[i] - 1) r += C[n-1-x][k-1-i];
			p = c[i] + 1;
		}
		return r;
	}
	vector<int> unrankSubset(T r, int n, int k){
		vector<int> c;
		FOR(int, x, 0, n - 1) if (k){
			if (r < C[n-1-x][k-1]){ c.push_back(x); k--; }
			else r -= C[n-1-x][k-1];
		}
		return c;
	}
	T rankPerm(const vector<int> &p){
		T r{}; int n = p.size();
		FOR(int, i, 0, n - 1){
			int s = 0; FOR(int, j, i+1, n - 1) s += p[j] < p[i];
			r = r * T(n - i) + T(s);
		}
		return r;
	}
	vector<int> unrankPerm(T r, int n){
		vector<int> d(n), a(n), p;
		iota(a.begin(), a.end(), 0);
		REV(int, i, n - 1, 0){
			d[i] = r % (n - i); r = r / (n - i);
		}
		for (int x: d){
			p.push_back(a[x]); a.erase(a.begin() + x);
		}
		return p;
	}
};
