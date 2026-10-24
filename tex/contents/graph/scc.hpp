#include "../contest/template.hpp"

struct Node{
	int dest, idx;
	Node(int dest, int idx): dest(dest), idx(idx) {}
	Node(): Node(0, 0) {}
};
const int MAX = 500005;
int n, m;
vector<Node> graph[MAX];
namespace Tarjan {
	int low[MAX], num[MAX], time;
	vector<int> scc[MAX]; int root[MAX];
	stack<int> tmp;
	void dfs(int node) {
		low[node] = num[node] = ++time; tmp.push(node);
		for (Node ch: graph[node]) {
			int child = ch.dest;
			if (num[child]) 
				minimize(low[node], low[child]);
			else {
				dfs(child);
				minimize(low[node], num[child]);
			}
		}
		if (low[node] == num[node]) {
			int cur; do {
				cur = tmp.top(); tmp.pop();
				root[cur] = node; scc[node].push_back(cur);
			} while (cur != node);
		}
	}
}