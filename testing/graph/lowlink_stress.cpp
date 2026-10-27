#include "../../tex/contents/graph/lowlink.hpp"
int N; vector<pair<int,int>> E;
int comps(int skipV, int skipE){ // components count excluding vertex skipV, edge skipE
	vector<int> p(N); iota(p.begin(),p.end(),0);
	function<int(int)> f=[&](int x){return p[x]==x?x:p[x]=f(p[x]);};
	EFOR(int,i,0,(int)E.size()) if(i!=skipE){auto[a,b]=E[i]; if(a==skipV||b==skipV) continue; p[f(a)]=f(b);}
	int c=0; EFOR(int,i,0,N) if(i!=skipV&&f(i)==i) c++; return c;
}
bool biconn(int S){ // induced subgraph on S connected, no cut vertex
	vector<int> vs; EFOR(int,i,0,N) if(S>>i&1) vs.push_back(i);
	auto conn=[&](int rem){ vector<int> p(N); iota(p.begin(),p.end(),0);
		function<int(int)> f=[&](int x){return p[x]==x?x:p[x]=f(p[x]);};
		for(auto[a,b]:E) if((S>>a&1)&&(S>>b&1)&&a!=rem&&b!=rem) p[f(a)]=f(b);
		set<int> r; for(int v:vs) if(v!=rem) r.insert(f(v)); return r.size()<=1; };
	if(vs.size()==1){ for(auto[a,b]:E) if(a!=b&&(a==vs[0]||b==vs[0])) return false; return true; }
	if(vs.size()==2){ for(auto[a,b]:E) if(a!=b&&(S>>a&1)&&(S>>b&1)) return true; return false; }
	if(!conn(-1)) return false; for(int v:vs) if(!conn(v)) return false; return true;
}
int main(){
	mt19937 rng(1);
	for(int it=0; it<20000; it++){
		N=rng()%8+1; int M=rng()%(N*2+1); E.clear();
		Lowlink L(N);
		EFOR(int,i,0,M){int a=rng()%N,b=rng()%N; if(rng()%3==0&&!E.empty()) tie(a,b)=E[rng()%E.size()]; E.push_back({a,b}); L.addEdge(a,b);}
		L.run();
		int base=comps(-1,-1); bool ok=true;
		EFOR(int,v,0,N){ bool art=comps(v,-1)>base; if(art!=L.isArt[v]) ok=false; }
		EFOR(int,e,0,M){ bool br=comps(-1,e)>base; if(br!=L.isBridge[e]) ok=false; }
		// 2ecc: same comp in graph without bridges
		{ vector<int> p(N); iota(p.begin(),p.end(),0); function<int(int)> f=[&](int x){return p[x]==x?x:p[x]=f(p[x]);};
		  EFOR(int,e,0,M) if(comps(-1,e)==base) p[f(E[e].fi)]=f(E[e].se);
		  EFOR(int,a,0,N) EFOR(int,b,0,N) if((f(a)==f(b))!=(L.two[a]==L.two[b])) ok=false;
		  if(L.k2!=*max_element(L.two.begin(),L.two.end())+1) ok=false; }
		// bcc
		vector<int> good; EFOR(int,S,1,1<<N) if(biconn(S)) good.push_back(S);
		set<int> blocks; for(int S:good){ bool mx=true; for(int T:good) if(T!=S&&(T&S)==S) mx=false; if(mx) blocks.insert(S);}
		multiset<int> got; for(auto&c:L.bcc){int S=0; for(int v:c){ if(S>>v&1) ok=false; S|=1<<v;} got.insert(S);}
		if(multiset<int>(blocks.begin(),blocks.end())!=got) ok=false;
		if(!ok){ printf("FAIL n=%d\n",N); for(auto[a,b]:E) printf("%d %d\n",a,b); return 1;}
	}
	puts("OK");
}
