#include "../../miscellanous/template.hpp"

typedef unsigned long long ull;
struct modint{
	inline static /*const*/ ll MOD = 998244353;
	int v;
	static inline ll mod(ll num) {
		ll val=num-ull((__uint128_t(-1ULL/MOD)*num)>>64)*MOD;
		return val-(val>=MOD)*MOD;
	}
	modint inv() const{
		ll answer = 1, a = v, n = MOD - 2;
		while (n) {
			if (n & 1) answer = mod(answer * a);
			a = mod(a * a); n >>= 1;
		}
		return answer;
	}

	modint(ll a = 0)
		{ v=(a<0)?(MOD-mod(-a)):mod(a);v-=(v>=MOD)*MOD; }
	inline modint& operator += (modint b)
		{ v+=b.v; v-=(v>=MOD)*MOD; return *this; }
	inline modint& operator -= (modint b)
		{ v+=MOD-b.v; v-=(v>=MOD)*MOD; return*this; }
	inline modint& operator *= (modint b)
		{ v = mod(1ll * v * b.v); return *this; }
	inline modint& operator /= (modint b)
		{ return (*this)*=b.inv(); }
	inline modint& operator ^= (ll n) {
		modint a = v; v = 1;
		while (n) {if (n & 1) *this *= a; a *= a, n >>= 1;}
		return *this;
	}
};
#define NEWOP(op, op2) inline modint operator op\
(modint a, modint b) {return a op2 b;}
NEWOP(+,+=);NEWOP(-,-=);NEWOP(*,*=);NEWOP(/,/=);
#undef NEWOP
#define NEWCMP(op) inline bool operator op\
(modint a, modint b) {return a.v op b.v;}
NEWCMP(==);NEWCMP(!=);NEWCMP(<);NEWCMP(>);NEWCMP(<=);NEWCMP(>=);
#undef NEWCMP
inline modint operator - (modint a){return -a.v;}
inline modint operator^(modint a, ll n){return a^=n;}
inline istream& operator >> (istream& s, modint &i)
	{ ll tmp; s >> tmp; i = tmp; return s; }
inline ostream& operator << (ostream& s, modint i)
	{return s << i.v;}
