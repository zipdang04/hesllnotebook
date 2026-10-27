#pragma once
#include "../contest/template.hpp"

// M: S (values), F (maps); op associative with identity e; mp(f, x) = f(x); cp(f, g) = f after g; id
// t[k] = aggregate of node k with its own tag lz[k] already applied (lz[k] still owed to children)
template<class M> struct LazySeg{
	using S = typename M::S; using F = typename M::F;
	int n; vector<S> t; vector<F> lz;
	LazySeg(const vector<S>& a): n(a.size()), t(4 * n), lz(4 * n, M::id()){ if (n) _build(1, 1, n, a); } // pos i = a[i-1]
	void _build(int k, int l, int r, const vector<S>& a){
		if (l == r){ t[k] = a[l - 1]; return; }
		int m = (l + r) / 2; _build(2 * k, l, m, a); _build(2 * k + 1, m + 1, r, a);
		t[k] = M::op(t[2 * k], t[2 * k + 1]);
	}
	void _put(int k, F f){ t[k] = M::mp(f, t[k]); lz[k] = M::cp(f, lz[k]); }
	void _push(int k){ _put(2 * k, lz[k]); _put(2 * k + 1, lz[k]); lz[k] = M::id(); }
	void apply(int L, int R, F f){ _apply(1, 1, n, L, R, f); } // a[i] = f(a[i]), L <= i <= R
	void _apply(int k, int l, int r, int L, int R, F f){
		if (R < l || r < L) return;
		if (L <= l && r <= R) return _put(k, f);
		_push(k); int m = (l + r) / 2;
		_apply(2 * k, l, m, L, R, f); _apply(2 * k + 1, m + 1, r, L, R, f);
		t[k] = M::op(t[2 * k], t[2 * k + 1]);
	}
	S query(int L, int R){ return _query(1, 1, n, L, R); } // op(a[L..R]), e() if L > R
	S _query(int k, int l, int r, int L, int R){
		if (R < l || r < L) return M::e();
		if (L <= l && r <= R) return t[k];
		_push(k); int m = (l + r) / 2;
		return M::op(_query(2 * k, l, m, L, R), _query(2 * k + 1, m + 1, r, L, R));
	}
	// max R in [L-1, n] with g(query(L, R)); g monotone (true, ..., true, false, ...), g(e()) = true
	template<class G> int maxRight(int L, G g){ S a = M::e(); return _maxR(1, 1, n, L, g, a); }
	template<class G> int _maxR(int k, int l, int r, int L, G& g, S& a){
		if (L <= l && g(M::op(a, t[k]))) return a = M::op(a, t[k]), r;
		if (r < L || l == r) return r < L ? r : l - 1;
		_push(k); int m = (l + r) / 2, x = _maxR(2 * k, l, m, L, g, a);
		return x < m ? x : _maxR(2 * k + 1, m + 1, r, L, g, a);
	}
};
struct _exampleAddMin{ // range add, range min
	using S = ll; using F = ll;
	static S op(S x, S y){ return min(x, y); }
	static S e(){ return LLONG_MAX; }
	static S mp(F f, S x){ return x + f; }
	static F cp(F f, F g){ return f + g; }
	static F id(){ return 0; }
};
