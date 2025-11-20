#include "../../miscellanous/template.hpp"

#define MAX 1000001
struct Node
{
	int node, len;
	Node() {node = len = 0;}
	Node(int node, int len) {this -> node = node, this -> len = len;}
};
namespace SPFA{
int n; vector<Node> graph[MAX];
constexpr ll oo = (1e18+1);
bool in[MAX]; int cnt[MAX];
ll d[MAX], trace[MAX];
deque<int> q;
randomize;

void spfa(int root, bool detect){
	FOR(int, i, 0, n)
		shuffle(graph[i].begin(), graph[i].end(), mt);
	memset(d, 0x3f, sizeof(d));

	d[root] = 0; q.push_back(root);
	ll qSum = 0;
	while (!q.empty()) {
		while (d[q.front()] > d[q.back()])
			{ q.push_back(q.front()); q.pop_front(); }
		int node = q.front(); q.pop_front();
		in[node] = false;
		const int LIM = sqrt(n);
		qSum -= d[node];
		for (Node ch: graph[node]) {
			int child=ch.node;
			ll chDist=(d[node]==-oo) ? -oo :(d[node]+ch.len);
			if (chDist >= d[child]) continue;
			qSum=max(-oo,qSum+chDist-in[child]*d[child]);
			d[child] = chDist, trace[child] = node;
			if (in[child]) continue;
			if (++cnt[child] == n) {
				if (not detect) break;
				d[child] = chDist = -oo;
			}
			in[child] = true;
			if (cnt[child] > LIM or d[child] * q.size() > qSum)
				q.push_back(child);
			else if (q.empty() or d[child] <= d[q.front()])
				q.push_front(child);
			else
				q.push_back(child);
		}
	}
}
}
