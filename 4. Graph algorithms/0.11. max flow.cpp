//
// Created by akim on 13/04/2021.
//
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

const int MAX_N = 200000, INF = INT32_MAX;

struct edge {
    int to, capacity;
    long long flow;
};

int n, s, t, d[MAX_N], ptr[MAX_N], q[MAX_N];
vector<edge> e;
vector<int> g[MAX_N];

void add_edge(int a, int b, int cap) {
    edge e1 = { b, cap, 0 };
    edge e2 = { a, 0, 0 };
    g[a].push_back((int)e.size());
    e.push_back(e1);
    g[b].push_back((int)e.size());
    e.push_back(e2);
}

bool bfs() {
    int qh = 0, qt = 0;
    q[qt++] = s;
    memset(d, -1, n * sizeof d[0]);
    d[s] = 0;
    while (qh < qt && d[t] == -1) {
        int v = q[qh++];
        for (int i = 0; i < g[v].size(); ++i) {
            int id = g[v][i], to = e[id].to;
            if (d[to] == -1 && e[id].flow < e[id].capacity) {
                q[qt++] = to;
                d[to] = d[v] + 1;
            }
        }
    }
    return d[t] != -1;
}

long long dfs(int v, long long flow) {
    if (!flow)
        return 0;
    if (v == t)
        return flow;
    for (;ptr[v] < (int)g[v].size(); ++ptr[v]) {
        int id = g[v][ptr[v]], to = e[id].to;
        if (d[to] != d[v] + 1)
            continue;
        long long pushed = dfs(to, min(flow, e[id].capacity - e[id].flow));
        if (pushed) {
            e[id].flow += pushed;
            e[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

long long maxFlow() {
    long long flow = 0;
    while (bfs()) {
        memset(ptr, 0, n * sizeof ptr[0]);
        while (long long pushed = dfs(s, INF))
            flow += pushed;
    }
    return flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, u, v, w;
    cin >> n >> m;
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v >> w;
        add_edge(u - 1, v - 1, w);
    }

    cout << maxFlow();
    return 0;
}