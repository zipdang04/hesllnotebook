#include <bits/stdc++.h>
using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
*/

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
template <class T> using PQMax = priority_queue<T>;
template <class T> using PQMin = priority_queue<T, vector<T>, greater<T>>;
template <class T1, class T2>
void maximize(T1 &a, T2 b){
	if (b > a) a = b;
}
template <class T1, class T2>
void minimize(T1 &a, T2 b){
	if (b < a) a = b;
}
template <class T>
void read(T &number)
{
	bool negative = false;
	register int c;
	number = 0;
	c = getchar();
	while (c != '-' && !isalnum(c)) c = getchar();
	if (c=='-'){
		negative = true;
		c = getchar();
	}
	for (; (c>47 && c<58); c=getchar())
		number = number *10 + c - 48;
	if (negative)
		number *= -1;
}
template <class T, class ...Ts>
void read(T &a, Ts& ... args){
	read(a);
	read(args...);
}

struct Node
{
	int node, len;
	Node() {node = len = 0;}
	Node(int node, int len) {this -> node = node, this -> len = len;}
};
typedef vector<Node> vg;

#define fi first
#define se second

#define FOR(type, i, a, b) for(type i = (a); i <= (b); i++)
#define REV(type, i, b, a) for(type i = (b); i >= (a); i--)

#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcount(n)
#define cntBitll(n) __builtin_popcountll(n)
#define log2(n) (31 - __builtin_clz(n))
#define log2ll(n) (63 - __builtin_clzll(n))
#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

#define MAX 1000001
#define MOD 1000000007

int n; vector<Node> graph[MAX];
namespace SPFA{
	const ll oo = (1e18+1);
	bool inside[MAX]; int count[MAX];
	ll dist[MAX], trace[MAX]; deque<int> q;
	
	void spfa(int root){
		memset(dist, -0x3f, sizeof(dist));
		dist[root] = 0; q.push_back(root); 
		ll sum = 0, sz = 0;
		while (!q.empty()) {
			int node = q.front(); q.pop_front(); 
			inside[node] = false; sum -= dist[node], sz--;
			for (Node ch: graph[node]) {
				int child = ch.node; ll chDist = min(oo, dist[node] + ch.len);
				if (dist[child] < chDist) {
					sum = max(oo, sum + chDist - inside[child] * dist[child]);
					dist[child] = chDist, trace[child] = node;
					if (inside[child]) continue; 
					if (++count[child] == n)
						dist[child] = chDist = oo;
					sz++, inside[child] = true; 
					if (chDist * sz < sum) q.push_front(child);
					else q.push_back(child);
				}
			}
		}
	}

	vector<int> findCycle() {
		int start = 1; while (count[start] < n) start++;
		vector<int> ans {start}; int node = trace[start];
		while (node != start) ans.push_back(node), node = trace[node];
		return ans;
	}
}

int m;

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	cin >> n >> m;
	FOR(int, _, 1, m) {
		int u, v, w; cin >> u >> v >> w;
		graph[u].emplace_back(v, w);
	}
	SPFA::spfa(1);
	if (SPFA::dist[n] < SPFA::oo) 
		cout << SPFA::dist[n];
	else
		cout << -1;
}