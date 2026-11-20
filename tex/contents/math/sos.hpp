#pragma once
#include "../contest/template.hpp"

// sos: a[S] = f-merge of a[T] over T in S (sup: T over S);
// f(x, y) merges y into x: +=, maximize, top-2, -= inverts.
template<class T, class F>
void sos(vector<T> &a, F f, bool sup = false){
	for(int k = 1; k < sz(a); k *= 2)
		FOR(S, 0, sz(a) - 1)
			if(S & k) sup ? f(a[S ^ k], a[S]) : f(a[S], a[S ^ k]);
}
