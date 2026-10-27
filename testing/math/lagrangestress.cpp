#include "../../tex/contents/math/lagrange.hpp"
mt19937_64 rng(1);
ll rnd(ll l,ll r){return uniform_int_distribution<ll>(l,r)(rng);}
modint ev(const vector<modint>&c, modint x){modint r=0;REV(int,j,(int)c.size()-1,0) r=r*x+c[j];return r;}
int main(){
	for(ll M: {998244353LL, 1000000007LL, 7LL, 13LL, 2LL}){
		modint::MOD=M;
		int bad=0;
		FOR(int,n,1,(int)min<ll>(M,40)) EFOR(int,it,0,30){
			vector<modint> c(n),y(n),x;
			for(auto&v:c)v=rnd(0,M-1);
			EFOR(int,i,0,n) y[i]=ev(c,i);
			ll X=it%3?rnd(0,M-1):rnd(0,(ll)4e18);
			if(lagrangeAt(y,X)!=ev(c,X)){bad++;printf("at M=%lld n=%d\n",M,n); return 1;}
			vector<ll> pool; if(M<=1000){pool.resize(M);iota(pool.begin(),pool.end(),0);}
			if(M<=1000) { shuffle(pool.begin(),pool.end(),rng); for(int i=0;i<n;i++)x.push_back(pool[i]);}
			else { set<ll> s; while((int)s.size()<n)s.insert(rnd(0,M-1)); for(ll v:s)x.push_back(v); shuffle(x.begin(),x.end(),rng);}
			vector<modint> y2(n); EFOR(int,i,0,n) y2[i]=ev(c,x[i]);
			if(interpolate(x,y2)!=c){bad++;printf("interp M=%lld n=%d\n",M,n); return 1;}
		}
		printf("M=%lld bad=%d\n",M,bad);
		if(bad>0) return 1;
	}
	// timing
	modint::MOD=998244353; int n=2000; vector<modint> x(n),y(n); EFOR(int,i,0,n){x[i]=i*7+1;y[i]=rnd(0,1e9);}
	auto c=interpolate(x,y); EFOR(int,i,0,n) if(ev(c,x[i])!=y[i]){ puts("tbad"); return 1; }
	vector<modint> yy(100000); for(auto&v:yy)v=rnd(0,1e9); cout<<lagrangeAt(yy,12345678901LL)<<endl;
	puts("ok");
	return 0;
}
