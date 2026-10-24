#include "../contest/template.hpp"
class TWOSET{
private:
	inline static constexpr int oo = 1e9;
	int n, all, m, cntGr = 0, time = 0;
	vector<vector<int>> graph; vector<int> grIdx, low, num;
	vector<bool> answer; stack<int> st;

	inline int getIdx(int u){return (u > 0) ? u : (-u + n);};
	void initTarjan()
		{ low = num = vector<int>(all, false); time=cntGr=0; }
	void tarjan(int node){
		low[node] = num[node] = ++time; st.push(node);
		for (int child: graph[node])
			if (num[child]) minimize(low[node], num[child]);
			else{tarjan(child); minimize(low[node],low[child]);}
		if (num[node] == low[node]){
			++cntGr; int v = -1;
			while (v != node){
				v=st.top();st.pop();
				grIdx[v]=cntGr;low[v]=num[v]=oo;
			} } }
public:
	TWOSET(){}
	void init(int n){
		this -> n = n; m = 0; all = n * 2 + 1;
		graph = vector<vector<int>>(all, vector<int>());
		grIdx = vector<int>(all, 0);
	}
	void addEdge(int u, int v){
		graph[getIdx(-u)].push_back(getIdx(v));
		graph[getIdx(-v)].push_back(getIdx(u));
	}
	bool checkSatisfiable(){
		initTarjan();
		for (int node = 1; node < all; node++)
			if (low[node] == 0) tarjan(node);
		answer = vector<bool>(n + 1);
		for (int i = 1; i <= n; i++){
			int u = getIdx(i), v = getIdx(-i);
			if (grIdx[u] == grIdx[v]) return false;
			answer[i] = grIdx[u] < grIdx[v];
		}
		return true;
	}
	bool getAnswer(int u){ return answer[u]; }
};
