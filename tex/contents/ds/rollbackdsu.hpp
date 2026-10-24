#pragma once
#include "../contest/template.hpp"

// union by size, no path compression: find O(log n); int t = time(); ...; rollback(t) undoes joins after t
struct RollbackDSU{
	vector<int> e; vector<pair<int,int>> st; // e[x] < 0: x is a root of size -e[x]
	RollbackDSU(int n): e(n, -1) {}
	int find(int x){ return e[x] < 0 ? x : find(e[x]); }
	int size(int x){ return -e[find(x)]; }
	int time(){ return st.size(); }
	void rollback(int t){
		while ((int)st.size() > t) e[st.back().fi] = st.back().se, st.pop_back();
	}
	bool join(int a, int b){ // false if already connected
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.push_back({a, e[a]}); st.push_back({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};
