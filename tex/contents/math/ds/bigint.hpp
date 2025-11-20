#include "../../miscellanous/template.hpp"
#include "../../math/numerical/ntt.hpp"

// potential MLE in reserve() && resize()
class bigint {
using vc = vector<char>;
private:
	bool neg = false;
	vc ds;
	inline bigint&flipNega(){neg=!neg;return *this;}
	inline void reformat() {
		while ((int)ds.size() > 1 && *(ds.rbegin()) == 0) ds.pop_back();
		if (ds.size() == 1 && ds[0] == 0) neg = false; }
	inline bigint& _pD(const bigint&num, int p10=0) {
		resize(max(size(), num.size() + p10)); bool nho = 0;
		EFOR(int, i, p10, num.size() + p10) {
			ds[i] += nho + num.ds[i-p10];
			nho = ds[i] > 9; ds[i] -= 10 * nho; }
		EFOR(int, i, num.size() + p10, size()) {
			ds[i]+=nho; nho=ds[i]>9; ds[i]-=10*nho;
			if (nho == 0) break; }
		if (nho) push_back(nho); return *this; }
	inline bool ltAbs(const bigint&oth,bool xV,bool eqV)const{
		if (size()!=oth.size())return size()<oth.size();
		REV(int, i, size()-1, 0) if (ds[i] != oth.ds[i])
			return (ds[i] < oth.ds[i]) xor xV;
		return eqV; }
	inline bigint& _mD(const bigint &num) {
		bool flip = ltAbs(num, false, false);
		const vc *big = &(flip ? num.ds : ds),
				 *small = &(flip ? ds : num.ds);
		resize(big->size());

		bool nho = 0;
		EFOR(int, i, 0, small->size()) {
			ds[i] = (*big)[i] - (nho + (*small)[i]);
			nho = ds[i] < 0; ds[i] += 10 * nho; }
		EFOR(int, i, small->size(), size()) {
			ds[i] = (*big)[i] - nho; nho = ds[i] < 0;
			ds[i] += 10 * nho; if (nho == 0) break; }
		if (nho or flip) flipNega();
		if (nho) push_back(nho); else reformat();
		return *this; }

public:
	inline static string _I;
	/* constructors */
	bigint() { ds = {0}; }
	bigint(ll num) {
		if(num == 0){ds={0};return;} ds.reserve(18);
		while (num) ds.push_back(num%10), num/=10; }
	bigint(string s) {
		ds.reserve(s.size());
		REV(int, j, (int)s.size()-1, 0)
			if (s[j] != '-') ds.push_back(s[j] - '0');
			else neg = true;
		reformat(); }
	bigint(vc &ds, bool neg):ds(ds),neg(neg){reformat();}
	bigint(vc &&ds, bool neg):ds(ds),neg(neg){reformat();}
	/* access data */
	inline bool isN() const { return neg; }
	inline char&operator[](int idx) { return ds[idx]; }
	/* copy vector api */
	inline int size() const { return ds.size(); }
	inline bool empty() const { return ds.empty(); }
	inline void pop_back(){ ds.pop_back(); }
	inline void reserve(int sz) { ds.reserve(sz); }
	inline void resize(int sz) { ds.resize(sz); }
	inline void push_back(char c){ ds.push_back(c); }
	/* comparator */
	inline bool operator == (const bigint& oth) const {
		if(isN()!=oth.isN()||size()!=(int)oth.size())return 0;
		REV(int,i,size()-1,0)if(ds[i]!=oth.ds[i])return false;
		return true; }
	inline bool operator < (const bigint& oth) const
		{ return(isN()!=oth.isN())?isN():ltAbs(oth,isN(),0); }
	inline bool operator <= (const bigint& oth) const
		{ return(isN()!=oth.isN())?isN():ltAbs(oth,isN(),1); }
	inline bool operator != (const bigint &num) const
		{ return not operator==(num); }
	inline bool operator >= (const bigint &num) const
		{ return not operator<(num); }
	inline bool operator > (const bigint &num) const
		{ return not operator<=(num); }
	// fix rlvalue error/improve performance by:
	// inline bool operator ...  (bigint &&num) const
	// 	{ return operator ... (num); }

	/* operator + - * / */
	inline bigint operator - () const
		{ return bigint(*this).flipNega(); }
	inline bigint& operator += (const bigint& num)
		{return (num.isN()==isN())?_pD(num):_mD(num);}
	inline bigint& operator -= (const bigint& num)
		{return (num.isN() == isN())?_mD(num):_pD(num);}
	inline bigint operator + (const bigint& num)
		{ bigint res(*this); res += num; return res; }
	inline bigint operator - (const bigint& num)
		{ bigint res(*this); res -= num; return res; }
	inline bigint operator * (const bigint &num) const {
		if (*this==0||num==0||empty()||num.empty())return 0;
		if (size() == 1 or num.size() == 1) {
			const vector<char> &D =(size()==1)?num.ds:ds;
			char mul = (size() == 1) ? ds[0] : num.ds[0];
			bigint ans; ans.resize(D.size()); char nho = 0;
			EFOR(int, i, 0, D.size()) {
				ans[i] = D[i] * mul + nho; nho = ans[i] / 10;
				ans[i] -= 10 * nho;
			}
			if (nho) ans.push_back(nho);
			if (isN() xor num.isN()) ans.flipNega();
			return ans;
		}
		/* FFT */
		FFT::buildRoot();
		vector<int> a(ds.begin(), ds.end());
		vector<int> b(num.ds.begin(), num.ds.end());
		vector<modint> newAns = FFT::multiply(a, b);
		bigint ans; ans.resize(newAns.size());
		int nho = 0;
		EFOR(int, i, 0, ans.size()){
			int tmp = newAns[i].v + nho;
			nho = tmp / 10;
			ans[i] = tmp - nho * 10;
		}
		while (nho>0) {ans.push_back(nho%10); nho/=10;}
		/* Karatsuba */
		// int mxSz = max(size(), num.size()), B = mxSz / 2;
		// bigint a0(vector<char>(ds.begin(), ds.begin() + min(B, size())), false),
		// a1(vector<char>(ds.begin()+min(B, size()), ds.end() ), false),
		// b0(vector<char>(num.ds.begin(), num.ds.begin()+min(B, num.size())), false),
		// b1(vector<char>(num.ds.begin()+min(B, num.size()), num.ds.end() ), false);
		//ans=a0*b0;bigint z2=a1*b1,z1=(a0+a1)*(b0+b1)-(z2+ans);
		// ans._pD(z2, B*2)._pD(z1, B);
		if (isN() xor num.isN()) ans.flipNega();
		ans.reformat(); return ans;
	}
	friend istream& operator>>(istream&inp, bigint&num)
		{inp>>bigint::_I;num=bigint(bigint::_I);return inp;}
	friend ostream&operator<<(ostream&out,const bigint&num){
		if (num.isN()) out << '-';
		REV(int,i,(int)num.size()-1,0)out<<char(num.ds[i]+'0');
		return out;
	}
};
