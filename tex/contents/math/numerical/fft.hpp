#include "../../miscellanous/template.hpp"

#define MAX 1000001
#define MOD 1000000007

typedef complex<ld> cd;
namespace FFT{
	const ld TAU = acos(-1) * 2;
		// 3.14159'26535'89793'23846'26433'83279'50288'41971
		// '69399'37510'58 * 2;
	
	constexpr int BIT = 20, MAX_LEN = 1 << BIT;
	vector<int> _rev[BIT + 1];
	cd _root[MAX_LEN + 1];
	void buildRoot() {
		_root[0] = _root[MAX_LEN] = 1;
		for (int i=BIT-1, dist=1<<(BIT-1); i>=0; i--, dist>>=1) {
			cd w = polar(ld(1.0), TAU * dist / MAX_LEN);
			for (int pos = 0; pos < MAX_LEN; pos += 2 * dist)
				_root[pos + dist] = _root[pos] * w;
		}
		_rev[0] = {0};
		for (int bit=1, len=2; len<=MAX_LEN; bit++, len*=2) {
			_rev[bit].resize(len, 0);
			for (int i = 0; i < len; i++)
				_rev[bit][i]=((i&1)<<(bit-1))|_rev[bit-1][i/2];
		}
	}
	void dft(vector<cd> &poly, bool invert = false) { 
		assert((cntBit((int)poly.size()) == 1));
		const int n = poly.size(), coef = MAX_LEN / n;
		for (int dist=n/2, span=n; dist>0; dist/=2, span/=2) 
		for (int pos = 0; pos < dist; pos++) 
		for (int i=pos, k=0; i<n; i+=span, k+=dist) {
			int len = log2(n / span);  
			int newK = _rev[len][k / dist] * dist;
			int tmp = (invert ? (n - newK) : newK) * coef;
			cd a = poly[i], b = _root[tmp] * poly[i + dist];
			poly[i] = a + b, poly[i + dist] = a - b;
		}
		if (invert) for (cd &x: poly) x /= n;
	}
	template<class T>vector<T>conv(vector<T>_a,vector<T>_b){
		int len = int(_a.size() + _b.size()) - 1; 
		int bit = log2(len)+(cntBit(len)>1); len=1<<bit;
		vector<cd> a(len);
		for (int i = 0; i < len; i++) 
			a[i] = cd((i < _a.size())?_a[i]:0, (i<_b.size())?_b[i]:0);
		dft(a, false);
		for (int i = 0; i < len; i++)
			if (i < _rev[bit][i]) swap(a[i], a[_rev[bit][i]]);
		for (int i = 0; i < len; i++) a[i] *= a[i];
		vector<cd> b(a.begin(), a.end());
		for (int i = 0; i < len; i++)
			b[i] = a[i] - conj(a[-i & (len-1)]); //(n-i)%n
		dft(b, true);
		for (int i = 0; i < len; i++)
			if (i < _rev[bit][i]) swap(b[i], b[_rev[bit][i]]);

		while (len > 0 && b[len - 1].imag() < 1e-9) len--;
		vector<T> ans(len); 
		FOR(int, i, 0, len-1) ans[i]=round(b[i].imag()/4.0);
		return ans;
	}
}