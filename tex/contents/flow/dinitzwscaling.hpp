#pragma once
#include "../contest/template.hpp"

struct Flow{
    struct Edge{int dest, rev; ll capa;};
    const ll oo = 1ll << 60;
    vector<vector<Edge>> adj;
    vector<pair<int,int>> where;
    int n, s, t;
    vector<int> level, ptr;
    void init(int n, int s, int t){ this->n=n; this->s=s; this->t=t; adj.assign(n,{}); where.clear(); level.resize(n); ptr.resize(n); }
    void addEdge(int u, int v, ll c){
        where.push_back({u, (int)adj[u].size()});
        adj[u].push_back({v, (int)adj[v].size(), c});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }
    bool bfs(ll lim){
        fill(level.begin(), level.end(), -1);
        queue<int> q; q.push(t); level[t] = 0;
        while (!q.empty() && level[s] < 0){
            int v = q.front(); q.pop();
            for (auto &e: adj[v])
                if (level[e.dest] < 0 && adj[e.dest][e.rev].capa >= lim)
                    level[e.dest] = level[v] + 1, q.push(e.dest);
        }
        return level[s] >= 0;
    }
    ll dfs(int v, ll f, ll lim){
        if (v == t) return f;
        ll pushed = 0;
        for (int &i = ptr[v]; i < (int)adj[v].size(); i++){
            Edge &e = adj[v][i];
            if (e.capa < lim || level[e.dest] != level[v] - 1) continue;
            ll got = dfs(e.dest, min(f - pushed, e.capa) / lim * lim, lim);
            if (got){ e.capa -= got; adj[e.dest][e.rev].capa += got; pushed += got; if (pushed == f) return pushed; }
        }
        return pushed;
    }
    ll calculate(){
        ll ans = 0;
        for (ll lim = oo; lim; lim >>= 1)
            while (bfs(lim)){ fill(ptr.begin(), ptr.end(), 0); ans += dfs(s, oo, lim); }
        return ans;
    }
    ll capOf(int id){ return adj[where[id].first][where[id].second].capa; }
};
