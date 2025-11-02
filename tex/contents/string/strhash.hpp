#include "../miscellanous/template.hpp"

class Hash {
	#define op operator
private:
	inline static constexpr int SZ = 2, MOD[6] = {
		(int) 1e9+7, (int) 1e9+20041203, (int) 1e9+2277,
		(int) 1e9+5277, (int) 1e9+8277, (int) 1e9+9277,
	};
	int val[SZ] = {};
	static ll pw(ll a, ll n, ll mod) {
		ll answer = 1;
		for (; n > 0; a = a * a % mod, n >>= 1)
			if (n & 1) answer = answer * a % mod;
		return answer;
	}
public:
	Hash(ll num){EFOR(int,i,0,SZ)val[i]=num%MOD[i];}
	Hash(): Hash(0) {}
	int& op[] (int idx) {return val[idx];}
	bool op == (const Hash &oth) const {
		EFOR(int,i,0,SZ)if(oth.val[i]!=val[i])return false;
		return true;
	}
	bool op < (const Hash &oth) const {
		EFOR(int,i,0,SZ)
			if(oth.val[i]!=val[i]) return val[i]<oth.val[i];
		return false;
	}
	bool op <= (const Hash &oth) const {
		EFOR(int, i, 0, SZ)
			if(oth.val[i]!=val[i])return val[i]<oth.val[i];
		return true;
	}
	bool op != (const Hash&oth)const{return not op==(oth);}
	bool op > (const Hash&oth)const{return not op<=(oth);}
	bool op >= (const Hash&oth)const{return not op<(oth);}
	Hash& op += (Hash &oth) {
		EFOR(int,i,0,SZ)val[i]=(val[i]+oth[i])%MOD[i];
		return *this;
	}
	Hash& op -= (Hash &oth) {
		EFOR(int,i,0,SZ)val[i]=(val[i]+MOD[i]-oth[i])%MOD[i];
		return *this;
	}
	Hash& op *= (Hash &oth) {
		EFOR(int,i,0,SZ)val[i]=(1ll*val[i]*oth[i])%MOD[i];
		return *this;
	}
	Hash inv() const {
		Hash ans; EFOR(int, i, 0, SZ)
			ans[i] = pw(val[i], MOD[i]-2, MOD[i]);
		return ans;
	}
	Hash& op/=(const Hash&oth){return op*=(oth.inv());}
	Hash& op+=(Hash&&oth){ return op+=(oth);}
	Hash& op-=(Hash&&oth){ return op-=(oth);}
	Hash& op*=(Hash&&oth){ return op*=(oth);}
	Hash& op/=(Hash&&oth){ return op/=(oth);}
	friend Hash op+(Hash a,Hash b){return(a += b);}
	friend Hash op-(Hash a,Hash b){return(a -= b);}
	friend Hash op*(Hash a,Hash b){return(a *= b);}
	friend Hash op/(Hash a,Hash b){return(a /= b);}
	friend ostream& op << (ostream& out, const Hash &num) {
		out << "(";
		EFOR(int,i,0,SZ)out<<num.val[i]<<",)"[i==SZ-1];
		return out;
	}
};

class HashString {
private:
	inline static constexpr int MAX = 1e6, BASE = 311;
	inline static Hash _p[MAX + 1], _i[MAX + 1];
	Hash *sF, *sB; int len;
	inline static bool _init = false;
public:
	static void init() {
		_init = true; _p[0] = 1;
		FOR(int, i, 1, MAX) _p[i] = _p[i-1] * BASE;
		_i[MAX] = _p[MAX].inv();
		REV(int, i, MAX-1, 0) _i[i] = _i[i + 1] * BASE;
		FOR(int, i, 0, MAX) assert(_i[i] * _p[i] == 1);
	}
	HashString(string s = "") {
		if (not _init) init(); len = s.size();
		sF=new Hash[len+1],sB=new Hash[len+2];
		sF[0]=sB[len+1]=0;
		FOR(int,i,1,len)sF[i]=sF[i-1]+s[i-1]*_p[i];
		REV(int,i,len,1)sB[i]=sB[i+1]+_p[len-i+1]*s[i-1];
	}
	~HashString() {}

	int getLength() const {return len;}
	Hash getF(int l,int r){return(sF[r]-sF[l-1])*_i[l-1];}
	Hash getB(int l,int r){return(sB[l]-sB[r+1])*_i[len-r];}
	Hash getF(int idx) {return sF[idx];}
	Hash getB(int idx) {return sB[idx];}
};
