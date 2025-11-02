#include "../../miscellanous/template.hpp"
#include "../../math/numerical/ntt.hpp"

// potential MLE in reserve() && resize()
class bigint {
	using vc = vector<char>;
	private:
		bool nega = false;
		vc digs;
		inline bigint&flipNega(){nega=!nega;return *this;}
		inline void reformat() {
			while ((int)digs.size() > 1 && *(digs.rbegin()) == 0) digs.pop_back();
			if (digs.size() == 1 && digs[0] == 0) nega = false;
		}
		inline bigint& _plusD(const bigint&num, int p10=0) {
			resize(max(size(), num.size() + p10)); bool nho = 0;
			EFOR(int, i, p10, num.size() + p10) {
				digs[i] += nho + num.digs[i-p10]; 
				nho = digs[i] > 9; digs[i] -= 10 * nho;
			}
			EFOR(int, i, num.size() + p10, size()) {
				digs[i]+=nho; nho=digs[i]>9; digs[i]-=10*nho; 
				if (nho == 0) break;
			}
			if (nho) push_back(nho); return *this;
		}
		inline bool ltAbs(const bigint&oth,bool xV,bool eqV)const{
			if (size()!=oth.size())return size()<oth.size();
			REV(int, i, size()-1, 0) if (digs[i] != oth.digs[i])
				return (digs[i] < oth.digs[i]) xor xV;
			return eqV;
		}
		inline bigint& _minusD(const bigint &num) {
			bool flip = ltAbs(num, false, false);
			const vc *big = &(flip ? num.digs : digs), 
					 *small = &(flip ? digs : num.digs);
			resize(big->size());
			
			bool nho = 0;
			EFOR(int, i, 0, small->size()) {
				digs[i] = (*big)[i] - (nho + (*small)[i]); 
				nho = digs[i] < 0; digs[i] += 10 * nho;
			}
			EFOR(int, i, small->size(), size()) {
				digs[i] = (*big)[i] - nho; nho = digs[i] < 0; 
				digs[i] += 10 * nho; if (nho == 0) break;
			}
			if (nho or flip) flipNega();
			if (nho) push_back(nho); else reformat();
			return *this;
		}
		
	public:
		inline static string _I;
		/* constructors */
		bigint() { digs = {0}; }
		bigint(ll num) {
			if(num == 0){digs={0};return;} digs.reserve(18);
			while (num) digs.push_back(num%10), num/=10;
		}
		bigint(string s) {
			digs.reserve(s.size());
			REV(int, j, (int)s.size()-1, 0)
				if (s[j] != '-') digs.push_back(s[j] - '0');
				else nega = true;
			reformat();
		}
		bigint(vc &digs, bool nega)
			: digs(digs), nega(nega) {reformat();}
		bigint(vc &&digs, bool nega)
			: digs(digs), nega(nega) {reformat();}
		/* access data */
		inline bool isNega() const { return nega; }
		inline char&operator[](int idx) { return digs[idx]; }
		/* copy vector api */
		inline int size() const { return digs.size(); }
		inline bool empty() const { return digs.empty(); }
		inline void pop_back(){ digs.pop_back(); }
		inline void reserve(int sz) { digs.reserve(sz); }
		inline void resize(int sz) { digs.resize(sz); }
		inline void push_back(char c){ digs.push_back(c); }
		/* comparator */
		inline bool operator == (const bigint& oth) const {
			if (isNega() != oth.isNega()) return false;
			if (size() != (int)oth.size()) return false;
			REV(int,i,size()-1,0)if(digs[i]!=oth.digs[i])return false;
			return true;
		}
		inline bool operator < (const bigint& oth) const {
			if (isNega() != oth.isNega()) return isNega();
			return ltAbs(oth, isNega(), false);
		}
		inline bool operator <= (const bigint& oth) const {
			if (isNega() != oth.isNega()) return isNega();
			return ltAbs(oth, isNega(), true);
		}
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
		inline bigint& operator += (const bigint& num) {
			if (num.isNega() == isNega()) _plusD(num);
			else _minusD(num); return *this;
		}
		inline bigint& operator -= (const bigint& num) {
			if (num.isNega() == isNega()) _minusD(num);
			else _plusD(num); return *this;
		}
		inline bigint operator + (const bigint& num) 
			{ bigint res(*this); res += num; return res; }
		inline bigint operator - (const bigint& num) 
			{ bigint res(*this); res -= num; return res; }
		inline bigint operator * (const bigint &num) const {
			if (*this==0||num==0||empty()||num.empty())return 0;
			if (size() == 1 or num.size() == 1) {
				const vector<char> &D =(size()==1)?num.digs:digs;
				char mul = (size() == 1) ? digs[0] : num.digs[0];
				bigint ans; ans.resize(D.size()); char nho = 0;
				EFOR(int, i, 0, D.size()) {
					ans[i] = D[i] * mul + nho; nho = ans[i] / 10;
					ans[i] -= 10 * nho;
				}
				if (nho) ans.push_back(nho);
				if (isNega() xor num.isNega()) ans.flipNega();
				return ans;
			}
			/* FFT */
			FFT::buildRoot();
			vector<int> a(digs.begin(), digs.end());
			vector<int> b(num.digs.begin(), num.digs.end());
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
			// bigint a0(vector<char>(digs.begin(), digs.begin() + min(B, size())), false),
			// a1(vector<char>(digs.begin()+min(B, size()), digs.end() ), false),
			// b0(vector<char>(num.digs.begin(), num.digs.begin()+min(B, num.size())), false),
			// b1(vector<char>(num.digs.begin()+min(B, num.size()), num.digs.end() ), false);
			//ans=a0*b0;bigint z2=a1*b1,z1=(a0+a1)*(b0+b1)-(z2+ans);
			// ans._plusD(z2, B*2)._plusD(z1, B);
			if (isNega() xor num.isNega()) ans.flipNega();
			ans.reformat(); return ans;
		}
		friend istream& operator>>(istream&inp, bigint&num)
			{inp>>bigint::_I;num=bigint(bigint::_I);return inp;}
		friend ostream&operator<<(ostream&out,const bigint&num){
			if (num.isNega()) out << '-';
			REV(int,i,(int)num.size()-1,0)out<<char(num.digs[i]+'0');
			return out;
		}
		friend ostream& operator << (ostream& out, bigint&& num) {
			return out << num;
		}
};
