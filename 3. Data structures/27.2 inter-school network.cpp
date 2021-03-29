//
// Created by akim on 08.03.2021.
//
#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

static int current_color = 0;

struct vertex {
    int index, color;

    explicit vertex(int i) {
        index = i;
        color = -1;
    }
};
vector<vector<vertex*>> graph, g_transposed, components;
vector<int> order;
vector<vertex*> all_vertexes;

vector<pair<int, unordered_set<int>>> cond;
unordered_set<int>* cond_transposed;
vector<int> sources, sinks, isolated_vertexes, paired_sources, paired_sinks;

bool* used;
void makeUnused(int n) {
    for(int i = 0; i < n; ++i)
        used[i] = false;
}

void dfs1(vertex* v) {
    used[v->index] = true;
    for (auto& to : graph[v->index])
        if (!used[to->index])
            dfs1(to);
    order.push_back(v->index);
}
void dfs2 (vertex* v) {
    used[v->index] = true;
    components[current_color].push_back(v);
    v->color = current_color;
    for (auto& to : g_transposed[v->index])
        if (!used[to->index])
            dfs2(to);
}
void createCondensation(int n) {
    int cc_number = 0;
    for (auto& i : all_vertexes)
        if (!used[i->index]) {
            dfs1(i);
            cc_number++;
        }
    cout << cc_number << '\n';
    makeUnused(n);
    for (auto& i : all_vertexes) {
        int v = order[n - 1 - i->index];
        if (!used[v]) {
            dfs2(all_vertexes[v]);
            current_color++;
        }
    }
    cond.reserve(current_color);
    cond_transposed = new unordered_set<int>[current_color];
    sources.reserve(current_color);
    sinks.reserve(current_color);
    isolated_vertexes.reserve(current_color);
    for (int i = 0; i < current_color; ++i) {   //компоненты сильной связности
        cond[i].second.reserve(current_color);
        vertex* leader = components[i][0];
        for(vertex*& v : components[i])         //вершины в компоненте
            for (vertex*& to : graph[v->index]) //смежные вершины
                if (v->color != to->color) {
                    cond[i].second.insert(to->color);
                    cond_transposed[to->color].insert(i);
                }
        cond[i].first = leader->index + 1;
        if(cond[i].second.empty() && !cond_transposed[i].empty())
            sinks.push_back(i);
        else if(!cond[i].second.empty() && cond_transposed[i].empty())
            sources.push_back(i);
        else
            isolated_vertexes.push_back(i);
    }
}

int s, t, q, p, w;
bool is_sink(int v) {
    return cond[v].second.empty() && !cond_transposed[v].empty();
}
void findPair(int& x, bool& sink_not_found) {
    if(used[x])
        return;
    if (is_sink(x)) {
        w = x;
        sink_not_found = false;
    }
    used[x] = true;
    for(int y : cond[x].second)
        if(sink_not_found)
            findPair(y, sink_not_found);
}
void makePairs(int n) {
    makeUnused(n);
    paired_sources.reserve(n);
    paired_sinks.reserve(n);
    bool sink_not_found;
    for(int source : sources) {
        w = 0;
        sink_not_found = true;
        findPair(source, sink_not_found);
        if(w) {
            paired_sources.push_back(source);
            paired_sinks.push_back(w);
            p++;
        }
    }
    for(int source : sources)
        if(!used[source])
            paired_sources.push_back(source);
    s = paired_sources.size();
    for(int sin : sinks)
        if(!used[sin])
            paired_sinks.push_back(sin);
    t = paired_sinks.size();
    q = isolated_vertexes.size();
}

void printAugmentation() {
    cout << max(s + q, t + q) << '\n';
    if (s <= t && s) {
        for (int i = 0; i < p - 1; i++)
            cout << cond[paired_sinks[i]].first << ' ' << cond[paired_sources[i + 1]].first << '\n';
        for (int i = p; i < s; i++)
            cout << cond[paired_sinks[i]].first << ' ' << cond[paired_sources[i]].first << '\n';
        if (!q && s == t)
            cout << cond[paired_sinks[p - 1]].first << ' ' << cond[paired_sources[0]].first << '\n';
        if (s < t)
            cout << cond[paired_sinks[p - 1]].first << ' ' << cond[paired_sinks[s]].first << '\n';

        for (int i = s; i < t - 1; i++)
            cout << cond[paired_sinks[i]].first << ' ' << cond[paired_sinks[i + 1]].first << '\n';

        if (!q && s < t)
            cout << cond[paired_sinks[t - 1]].first << ' ' << cond[paired_sources[0]].first << '\n';
    }

    else if (s > t && t) {
        for (int i = 0; i < p - 1; i++)
            cout << cond[paired_sinks[i]].first << ' ' << cond[paired_sources[i + 1]].first << '\n';
        for (int i = p; i < t; i++)
            cout << cond[paired_sinks[i]].first << ' ' << cond[paired_sources[i]].first << '\n';
        if (!q && s == t)
            cout << cond[paired_sinks[p - 1]].first << ' ' << cond[paired_sources[0]].first << '\n';
        cout << cond[paired_sources[p - 1]].first << ' ' << cond[paired_sources[t]].first << '\n';
        for (int i = t; i < s - 1; i++)
            cout << cond[paired_sources[i]].first << ' ' << cond[paired_sources[i + 1]].first << '\n';
        if (!q)
            cout << cond[paired_sinks[t - 1]].first << ' ' << cond[paired_sources[0]].first << '\n';
    }

    else if (q && t) {
        if (s == t)
            cout << cond[paired_sinks[p - 1]].first << ' ' << cond[isolated_vertexes[0]].first << '\n';
        else
            cout << cond[paired_sinks[t - 1]].first << ' ' << cond[isolated_vertexes[0]].first << '\n';
        for (int i = 0; i < q - 1; i++)
            cout << cond[isolated_vertexes[i]].first << ' ' << cond[isolated_vertexes[i + 1]].first << '\n';
        cout << cond[isolated_vertexes[q - 1]].first << ' ' << cond[paired_sources[0]].first << '\n';
    }

    else if (q && !t) {
        for (int i = 0; i < q - 1; i++)
            cout << cond[isolated_vertexes[i]].first << ' ' << cond[isolated_vertexes[i + 1]].first << '\n';
        cout << cond[isolated_vertexes[q - 1]].first << ' ' << cond[isolated_vertexes[0]].first << '\n';
    }
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt","r", stdin);
    int n, j = -1;
    cin >> n;
    used = new bool[n]{false};
    graph.reserve(n);
    g_transposed.reserve(n);
    components.reserve(n);
    all_vertexes.reserve(n);
    for (int i = 0; i < n; ++i) {
        graph[i].reserve(n);
        g_transposed[i].reserve(n);
        components[i].reserve(n);
        all_vertexes.push_back(new vertex(i));
    }
    for (int i = 0; i < n; ++i) {
        while(true) {
            cin >> j;
            if(!j)
                break;
            graph[i].push_back(all_vertexes[j - 1]);
            g_transposed[j - 1].push_back(all_vertexes[i]);
        }
    }
    createCondensation(n);
    makePairs(n);
    printAugmentation();

    return 0;
}