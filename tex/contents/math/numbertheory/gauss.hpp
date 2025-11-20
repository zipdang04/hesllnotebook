#include "../../miscellanous/template.hpp"
#include "../ds/modint.hpp"
#define MAX 500
namespace Gaussian {
struct Equation {
	vector<modint> cf; modint sum; // cf coef
	void div(modint d){ for(modint&num:cf)num/=d;sum/=d; }
	void minus(Equation &eq, modint mul) {
		FOR(int,i,0,(int)cf.size()-1)cf[i]-=eq.cf[i]*mul;
		sum -= eq.sum * mul; } };
struct Elimination{
	int SZ=0;Elimination(){}Elimination(int SZ):SZ(SZ){}
	Equation eqs[MAX]; int cnt = 0; int col[MAX];

	void add(Equation eq) { eqs[cnt++] = eq; }
	int size() {return SZ;}		int count() {return cnt;}
	pair<vector<modint>, vector<vector<modint>>> solve() {
		int ln = 0;
		FOR(int, i, 0, size()-1) {
			int ptr = ln;
			while (ptr < count() and eqs[ptr].cf[i] == 0) ptr++;
			if (ptr==count())continue; swap(eqs[ln],eqs[ptr]);
			eqs[ln].div(eqs[ln].cf[i]);
			FOR(int, j, 0, count()-1) if (j != ln)
				eqs[j].minus(eqs[ln], eqs[j].cf[i]);
			col[ln++] = i; }
		FOR(int,i,ln,count()-1) if(eqs[i].sum!=0) return{};
		vector<modint> sol(SZ); vector<vector<modint>> basis;
		REV(int, i, size()-1, 0) {
			if (i != col[ln-1]) {
				vector<modint> cur(SZ); cur[i] = 1;
				FOR(int,row,0,ln-1)cur[col[row]]=-eqs[row].cf[i];
				basis.push_back(cur); continue; }
			sol[i] = eqs[--ln].sum; }
		return {sol, basis}; } }; }
