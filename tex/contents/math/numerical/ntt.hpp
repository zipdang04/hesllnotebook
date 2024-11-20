#include "../../math/ds/modint.hpp"

// MOD=998244353=c*2^k+1 => ROOT=g^c, any g: gcd(g,MOD)=1
namespace FFT{
	constexpr int K_MOD = 23, BIT = 20, MAX_LEN = 1 << BIT;
	constexpr int ROOT = 15311432;

	modint _root[MAX_LEN + 1];
	vector<int> _rev[BIT + 1];
	void buildRoot() {
		_root[0] = 1; modint mul=ROOT; 
		FOR(int, i, 1, K_MOD-BIT) mul*=mul;
		FOR(int, i, 1, MAX_LEN) _root[i] = _root[i-1] * mul;
		for (int bit=0, len=1; len<=MAX_LEN; bit++, len*=2) {
			_rev[bit].resize((1 << bit), 0);
			for (int i = 1; i < (1 << bit); i++)
				_rev[bit][i]=((i&1)<<(bit-1))|_rev[bit-1][i/2];
		}
	}

	void transform(vector<modint> &v, bool invert){
		const int len = v.size(), coef = MAX_LEN / len; 
		const int bit = log2(len); 
		FOR(int, i, 0, len-1) if (i < _rev[bit][i])
			swap(v[i], v[_rev[bit][i]]);
		
		for (int jmp=1, span=2; span<=len; jmp*=2, span*=2)
		for (int beg = 0; beg < len; beg += span)
		for (int i = 0; i < jmp; i++) {
			int k=coef*len/jmp*i/2; if (invert) k=MAX_LEN-k;
			modint a = v[beg+i], b = _root[k] * v[beg+i+jmp];
			v[beg + i] = a + b; v[beg + i + jmp] = a - b;
		}
		if (invert) FOR(int, i, 0, len-1) v[i] /= len;
	}
	template<class T>
	vector<modint> multiply(vector<T> &_a, vector<T> &_b) {
		int len = int(_a.size() + _b.size()) - 1;
		len = 1 << (log2(len) + (cntBit(len) > 1));
		vector<modint>a(_a.begin(),_a.end()); a.resize(len,0);
		vector<modint>b(_b.begin(),_b.end()); b.resize(len,0);
		transform(a, false); transform(b, false);
		FOR(int, i, 0, len - 1) a[i] *= b[i];
		transform(a, true); return a;
	}
}