//
// Created by akim on 08.03.2021.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

static int current_component = 0;

struct Vertex {
    int index, component;

    explicit Vertex(int i) {
        index = i;
        component = -1;
    }
};
vector<int> order;
vector<Vertex*> all_vertices, *g, *g_transposed, *components;

pair<int, unordered_set<int>> *cond;
unordered_set<int>* cond_transposed;
vector<int> sources, sinks, isolated_vertices, paired_sources, paired_sinks;

bool* used;
void makeUnused(int n) {
    for (int i = 0; i < n; ++i)
        used[i] = false;
}

void dfs1(Vertex* v) {
    used[v->index] = true;
    for (auto& to : g[v->index])
        if (!used[to->index])
            dfs1(to);
    order.push_back(v->index);
}
void dfs2(Vertex* v) {
    used[v->index] = true;
    components[current_component].push_back(v);
    v->component = current_component;
    for (auto& to : g_transposed[v->index])
        if (!used[to->index])
            dfs2(to);
}
void createCondensation(int n) {
    for (auto& i : all_vertices)
        if (!used[i->index])
            dfs1(i);
    makeUnused(n);
    for (auto& i : all_vertices) {
        int v = order[n - 1 - i->index];
        if (!used[v]) {
            dfs2(all_vertices[v]);
            current_component++;
        }
    }
    cond = new pair<int, unordered_set<int>>[current_component];
    cond_transposed = new unordered_set<int>[current_component];
    sources.reserve(current_component);
    sinks.reserve(current_component);
    isolated_vertices.reserve(current_component);
    for (int i = 0; i < current_component; ++i) {
        cond[i].second.reserve(current_component);
        Vertex* leader = components[i][0];
        for (Vertex*& v : components[i])
            for (Vertex*& to : g[v->index])
                if (v->component != to->component) {
                    cond[i].second.insert(to->component);
                    cond_transposed[to->component].insert(i);
                }
        cond[i].first = leader->index + 1;
    }
    for (int i = 0; i < current_component; ++i) {
        if (cond[i].second.empty() && !cond_transposed[i].empty())
            sinks.push_back(i);
        else if (!cond[i].second.empty() && cond_transposed[i].empty())
            sources.push_back(i);
        else if (cond[i].second.empty() && cond_transposed[i].empty())
            isolated_vertices.push_back(i);
    }
}

int s, t, q, p = 0, w;
bool isSink(int v) {
    return cond[v].second.empty() && !cond_transposed[v].empty();
}
void findPair(int& x, bool& sink_not_found) {
    if (used[x])
        return;
    if (isSink(x)) {
        w = x;
        sink_not_found = false;
    }
    used[x] = true;
    for (int y : cond[x].second)
        if (sink_not_found)
            findPair(y, sink_not_found);
}

bool* used_sources, *used_sinks;
void makePairs(int n) {
    makeUnused(n);
    used_sources = new bool[n]{false};
    used_sinks = new bool[n]{false};
    paired_sources.reserve(n);
    paired_sinks.reserve(n);
    bool sink_not_found;
    for (int source : sources) {
        w = 0;
        sink_not_found = true;
        findPair(source, sink_not_found);
        if (w) {
            paired_sources.push_back(source);
            used_sources[source] = true;
            paired_sinks.push_back(w);
            used_sinks[w] = true;
            p++;
        }
    }
    for (int source : sources)
        if (!used_sources[source])
            paired_sources.push_back(source);
    for (int sin : sinks)
        if (!used_sinks[sin])
            paired_sinks.push_back(sin);
    s = paired_sources.size();
    t = paired_sinks.size();
    q = isolated_vertices.size();
}

void printAugmentation() {
    cout << s + q << '\n';
    int v1, v2;
    if(current_component > 1) {
        cout << max(s + q, t + q) << '\n';
        if (s <= t) {
            for (int i = 1; i < p; i++) {
                v1 = cond[paired_sinks[i - 1]].first;
                v2 = cond[paired_sources[i]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            for (int i = p + 1; i <= s; i++) {
                v1 = cond[paired_sinks[i - 1]].first;
                v2 = cond[paired_sources[i - 1]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            for (int i = s + 1; i < t; i++) {
                v1 = cond[paired_sinks[i - 1]].first;
                v2 = cond[paired_sinks[i]].first;
                cout << v1 << ' ' << v2 << '\n';
            }

            if (!q && s == t && s) {
                v1 = cond[paired_sinks[p - 1]].first;
                v2 = cond[paired_sources[0]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            if (!q && s < t && s) {
                v1 = cond[paired_sinks[t - 1]].first;
                v2 = cond[paired_sources[0]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            if (s < t && s) {
                v1 = cond[paired_sinks[p - 1]].first;
                v2 = cond[paired_sinks[s]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            if (q && t) {
                for (int i = 1; i < q; i++) {
                    v1 = cond[isolated_vertices[i - 1]].first;
                    v2 = cond[isolated_vertices[i]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
                if (s < t && t) {
                    v1 = cond[paired_sinks[t - 1]].first;
                    v2 = cond[isolated_vertices[0]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }

                if (s == t && t) {
                    v1 = cond[paired_sinks[p - 1]].first;
                    v2 = cond[isolated_vertices[0]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
                if (s) {
                    v1 = cond[isolated_vertices[q - 1]].first;
                    v2 = cond[paired_sources[0]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
            }
            if (q && !t) {
                for (int i = 1; i < q; i++) {
                    v1 = cond[isolated_vertices[i - 1]].first;
                    v2 = cond[isolated_vertices[i]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
                v1 = cond[isolated_vertices[q - 1]].first;
                v2 = cond[isolated_vertices[0]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
        }
        else {
            for (int i = 1; i < p; i++) {
                v1 = cond[paired_sinks[i - 1]].first;
                v2 = cond[paired_sources[i]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            for (int i = p + 1; i <= t; i++) {
                v1 = cond[paired_sinks[i - 1]].first;
                v2 = cond[paired_sources[i - 1]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            for (int i = t + 1; i < s; i++) {
                v1 = cond[paired_sources[i - 1]].first;
                v2 = cond[paired_sources[i]].first;
                cout << v1 << ' ' << v2 << '\n';
            }

            if (s > t) {
                v1 = cond[paired_sources[p - 1]].first;
                v2 = cond[paired_sources[t]].first;
                cout << v1 << ' ' << v2 << '\n';
            }
            if (!q && s > t) {
                v1 = cond[paired_sinks[t - 1]].first;
                v2 = cond[paired_sources[0]].first;
                cout << v1 << ' ' << v2 << '\n';
            }

            if (q) {
                for (int i = 1; i < q; i++) {
                    v1 = cond[isolated_vertices[i - 1]].first;
                    v2 = cond[isolated_vertices[i]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
                if (t) {
                    v1 = cond[paired_sinks[t - 1]].first;
                    v2 = cond[isolated_vertices[0]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
                if (s) {
                    v1 = cond[isolated_vertices[q - 1]].first;
                    v2 = cond[paired_sources[0]].first;
                    cout << v1 << ' ' << v2 << '\n';
                }
            }
        }
    }
    else
        cout << 0;
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/output.txt", "w", stdout);
    int n, j = -1;
    cin >> n;
    used = new bool[n] {false};
    g = new vector<Vertex*>[n];
    g_transposed = new vector<Vertex*>[n];
    components = new vector<Vertex*>[n];
    all_vertices.reserve(n);
    for (int i = 0; i < n; ++i) {
        g[i].reserve(n);
        g_transposed[i].reserve(n);
        components[i].reserve(n);
        all_vertices.push_back(new Vertex(i));
    }
    for (int i = 0; i < n; ++i) {
        while (true) {
            cin >> j;
            if (!j)
                break;
            g[i].push_back(all_vertices[j - 1]);
            g_transposed[j - 1].push_back(all_vertices[i]);
        }
    }
    createCondensation(n);
    makePairs(n);
    printAugmentation();
    return 0;
}