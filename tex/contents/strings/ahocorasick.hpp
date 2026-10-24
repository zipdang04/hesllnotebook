#pragma once
#include "../contest/template.hpp"

// patterns over the K characters first .. first+K-1; node 0 = root
template<int K = 26, char first = 'a'> struct AhoCorasick {
    struct Node { int nxt[K], link = 0; ll cnt = 0; Node() { fill(nxt, nxt + K, -1); } };
    vector<Node> t = vector<Node>(1);
    vector<int> order;                       // BFS order after build()

    int add(const string &s) {               // returns the node where s ends
        int u = 0;
        for (char ch: s) {
            int c = ch - first;
            if (t[u].nxt[c] < 0) { t[u].nxt[c] = t.size(); t.emplace_back(); }
            u = t[u].nxt[c];
        }
        t[u].cnt++;
        return u;
    }

    void build() {                           // failure links + goto table; cnt[u] = #patterns ending at u's suffixes
        queue<int> q; q.push(0);
        while (!q.empty()) {
            int u = q.front(); q.pop(); order.push_back(u);
            for (int c = 0; c < K; c++) {
                int &v = t[u].nxt[c], w = u ? t[t[u].link].nxt[c] : 0;
                if (v < 0) v = w;
                else { t[v].link = w; t[v].cnt += t[w].cnt; q.push(v); }
            }
        }
    }

    ll countMatch(const string &s) {         // total occurrences of all patterns in s
        ll r = 0; int u = 0;
        for (char ch: s) { u = t[u].nxt[ch - first]; r += t[u].cnt; }
        return r;
    }
};
