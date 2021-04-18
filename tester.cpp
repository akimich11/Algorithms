//
// Created by akim on 20.02.2021.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <unordered_set>

using namespace std;


class Solution1 {
public:
    ifstream in;
    ofstream out;
    Solution1() {
        in.open("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt");
        int n, j = -1;
        in >> n;
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
                in >> j;
                if (!j)
                    break;
                g[i].push_back(all_vertices[j - 1]);
                g_transposed[j - 1].push_back(all_vertices[i]);
            }
        }
        in.close();
        createCondensation(n);
        makePairs(n);
        printAugmentation();
    }
    int current_component = 0;

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
    void makeUnused(int n) const {
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
    bool isSink(int v) const {
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
        out.open("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/out.txt");
        out << s + q << '\n';
        int v1, v2;
        if(current_component > 1) {
            out << max(s + q, t + q) << '\n';
            if (s <= t) {
                for (int i = 1; i < p; i++) {
                    v1 = cond[paired_sinks[i - 1]].first;
                    v2 = cond[paired_sources[i]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                for (int i = p + 1; i <= s; i++) {
                    v1 = cond[paired_sinks[i - 1]].first;
                    v2 = cond[paired_sources[i - 1]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                for (int i = s + 1; i < t; i++) {
                    v1 = cond[paired_sinks[i - 1]].first;
                    v2 = cond[paired_sinks[i]].first;
                    out << v1 << ' ' << v2 << '\n';
                }

                if (!q && s == t && s) {
                    v1 = cond[paired_sinks[p - 1]].first;
                    v2 = cond[paired_sources[0]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                if (!q && s < t && s) {
                    v1 = cond[paired_sinks[t - 1]].first;
                    v2 = cond[paired_sources[0]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                if (s < t && s) {
                    v1 = cond[paired_sinks[p - 1]].first;
                    v2 = cond[paired_sinks[s]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                if (q && t) {
                    for (int i = 1; i < q; i++) {
                        v1 = cond[isolated_vertices[i - 1]].first;
                        v2 = cond[isolated_vertices[i]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                    if (s < t && t) {
                        v1 = cond[paired_sinks[t - 1]].first;
                        v2 = cond[isolated_vertices[0]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }

                    if (s == t && t) {
                        v1 = cond[paired_sinks[p - 1]].first;
                        v2 = cond[isolated_vertices[0]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                    if (s) {
                        v1 = cond[isolated_vertices[q - 1]].first;
                        v2 = cond[paired_sources[0]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                }
                if (q && !t) {
                    for (int i = 1; i < q; i++) {
                        v1 = cond[isolated_vertices[i - 1]].first;
                        v2 = cond[isolated_vertices[i]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                    v1 = cond[isolated_vertices[q - 1]].first;
                    v2 = cond[isolated_vertices[0]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
            }
            else {
                for (int i = 1; i < p; i++) {
                    v1 = cond[paired_sinks[i - 1]].first;
                    v2 = cond[paired_sources[i]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                for (int i = p + 1; i <= t; i++) {
                    v1 = cond[paired_sinks[i - 1]].first;
                    v2 = cond[paired_sources[i - 1]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                for (int i = t + 1; i < s; i++) {
                    v1 = cond[paired_sources[i - 1]].first;
                    v2 = cond[paired_sources[i]].first;
                    out << v1 << ' ' << v2 << '\n';
                }

                if (s > t) {
                    v1 = cond[paired_sources[p - 1]].first;
                    v2 = cond[paired_sources[t]].first;
                    out << v1 << ' ' << v2 << '\n';
                }
                if (!q && s > t) {
                    v1 = cond[paired_sinks[t - 1]].first;
                    v2 = cond[paired_sources[0]].first;
                    out << v1 << ' ' << v2 << '\n';
                }

                if (q) {
                    for (int i = 1; i < q; i++) {
                        v1 = cond[isolated_vertices[i - 1]].first;
                        v2 = cond[isolated_vertices[i]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                    if (t) {
                        v1 = cond[paired_sinks[t - 1]].first;
                        v2 = cond[isolated_vertices[0]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                    if (s) {
                        v1 = cond[isolated_vertices[q - 1]].first;
                        v2 = cond[paired_sources[0]].first;
                        out << v1 << ' ' << v2 << '\n';
                    }
                }
            }
        }
        else
            out << 0;
        out.close();
    }
};

class Solution2 {
public:
    Solution2() {
        ifstream in("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt");
        ofstream out("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/github_out.txt");
        int n;
        in >> n;
        vector<vector<int>> net(n);
        vector<vector<int>> t_net(n);
        vector<char> used(n);
        vector<int> component;
        vector<vector<int>> merged_graph(MAX_AMOUNT, vector<int>(MAX_AMOUNT));
        vector<vector<int>> components;
        for (int i = 0; i < n; i++) {
            int a = -1;
            while (a != 0) {
                in >> a;
                if (a != 0) {
                    net[i].push_back(a - 1);
                    t_net[a - 1].push_back(i);
                }
            }
        }
        in.close();
        used.assign(n, false);
        for (int i = 0; i < n; i++) {
            if (!used[i]){
                dfs1(net, i, used);
            }
        }
        used.assign(n, false);
        for (int i = n - 1; i >= 0; i--) {
            int v = order[i];
            if (!used[v]) {
                dfs2(v, used, component, t_net);
                components.push_back(component);
                component.clear();
            }
        }
        representators.resize(n);
        for (int i = 0; i < components.size(); i++) {
            int representator = components[i][0];
            unique_representators.push_back(representator);
            for (int j = 0; j < components[i].size(); j++) {
                representators[components[i][j]] = representator;
            }
        }
        for (int i = 0; i < components.size(); i++) {
            for (int j = 0; j < components[i].size(); j++) {
                for (int k = 0; k < net[components[i][j]].size(); k++) {
                    int element = net[components[i][j]][k];
                    merged_graph[components[i][j]][representators[element]] = 1;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            merged_graph[i][i] = 0;
        }



        for (int i = 0; i < unique_representators.size(); i++) {
            int temp1 = 0;
            int temp2 = 0;
            for (int j = 0; j < n; j++) {
                if (merged_graph[unique_representators[i]][j] == 0) {
                    temp1++;
                };
            }
            for (int k = 0; k < n; k++) {
                if (merged_graph[k][unique_representators[i]] == 0 || representators[k] == unique_representators[i]) {
                    temp2++;
                }
            }

            if (temp1 == n && temp2 != n) {
                sinks.push_back(unique_representators[i]);
            }

            if (temp2 == n && temp1 != n) {
                sources.push_back(unique_representators[i]);
            }
            if (temp2 == n && temp1 == n) {
                isolated.push_back(unique_representators[i]);
            }


        }

        int answer_a = sources.size() + isolated.size();
        int ans = fmax(sources.size(), sinks.size()) + isolated.size();

        marked.resize(n, false);
        w = -1;
        for (int i = 0; i < sources.size(); i++) {
            temporary.push_back(sources[i]);
        }
        added_sinks_sorted.resize(n, false);
        added_sources_sorted.resize(n, false);

        int appropriate_amount = strong_connectivity_sort(net, merged_graph);
        for (int i = 0; i < sources.size(); i++) {
            if (!added_sources_sorted[sources[i]]) {
                sources_sorted.push_back(sources[i]);
            }
        }
        for (int i = 0; i < sinks.size(); i++) {
            if (!added_sinks_sorted[sinks[i]]) {
                sinks_sorted.push_back(sinks[i]);
            }
        }
        int source_amount = sources_sorted.size();
        int sink_amount = sinks_sorted.size();
        int isolated_amount = isolated.size();

        int first = 0;
        int second = 0;

        if (source_amount <= sink_amount) {
            for (int i = 1; i < appropriate_amount; i++) {
                first = sinks_sorted[i - 1];
                second = sources_sorted[i];
                desired_edges.push_back(make_pair(first, second));
            }
            for (int i = appropriate_amount + 1; i <= source_amount; i++) {
                first = sinks_sorted[i - 1];
                second = sources_sorted[i - 1];
                desired_edges.push_back(make_pair(first, second));
            }
            for (int i = source_amount + 1; i < sink_amount; i++) {
                first = sinks_sorted[i - 1];
                second = sinks_sorted[i];
                desired_edges.push_back(make_pair(first, second));
            }

            if (isolated_amount == 0 && source_amount == sink_amount && source_amount != 0) {
                first = sinks_sorted[appropriate_amount - 1];
                second = sources_sorted[0];
                desired_edges.push_back(make_pair(first, second));
            }
            if (isolated_amount == 0 && source_amount < sink_amount && source_amount != 0) {
                first = sinks_sorted[sink_amount - 1];
                second = sources_sorted[0];
                desired_edges.push_back(make_pair(first, second));
            }
            if (source_amount < sink_amount && source_amount != 0) {
                first = sinks_sorted[appropriate_amount - 1];
                second = sinks_sorted[source_amount];
                desired_edges.push_back(make_pair(first, second));
            }
            if (isolated_amount != 0 && sink_amount != 0) {
                for (int i = 1; i < isolated_amount; i++) {
                    first = isolated[i - 1];
                    second = isolated[i];
                    desired_edges.push_back(make_pair(first, second));
                }
                if (source_amount < sink_amount && sink_amount != 0) {
                    first = sinks_sorted[sink_amount - 1];
                    second = isolated[0];
                    desired_edges.push_back(make_pair(first, second));
                }

                if (source_amount == sink_amount && sink_amount != 0) {
                    first = sinks_sorted[appropriate_amount - 1];
                    second = isolated[0];
                    desired_edges.push_back(make_pair(first, second));
                }
                if (source_amount != 0) {
                    first = isolated[isolated_amount - 1];
                    second = sources_sorted[0];
                    desired_edges.push_back(make_pair(first, second));
                }

            }
            if (isolated_amount != 0 && sink_amount == 0) {
                for (int i = 1; i < isolated_amount; i++) {
                    first = isolated[i - 1];
                    second = isolated[i];
                    desired_edges.push_back(make_pair(first, second));
                }
                first = isolated[isolated_amount - 1];
                second = isolated[0];
                desired_edges.push_back(make_pair(first, second));

            }


        }
        else if (source_amount > sink_amount) {
            for (int i = 1; i < appropriate_amount; i++) {
                first = sinks_sorted[i - 1];
                second = sources_sorted[i];
                desired_edges.push_back(make_pair(first, second));
            }
            for (int i = appropriate_amount + 1; i <= sink_amount; i++) {
                first = sinks_sorted[i - 1];
                second = sources_sorted[i - 1];
                desired_edges.push_back(make_pair(first, second));
            }
            for (int i = sink_amount + 1; i < source_amount; i++) {
                first = sources_sorted[i - 1];
                second = sources_sorted[i];
                desired_edges.push_back(make_pair(first, second));
            }

            if (source_amount > sink_amount) {
                first = sources_sorted[appropriate_amount - 1];
                second = sources_sorted[sink_amount];
                desired_edges.push_back(make_pair(first, second));
            }
            if (isolated_amount == 0 && source_amount > sink_amount) {
                first = sinks_sorted[sink_amount - 1];
                second = sources_sorted[0];
                desired_edges.push_back(make_pair(first, second));
            }

            if (isolated_amount != 0) {
                for (int i = 1; i < isolated_amount; i++) {
                    first = isolated[i - 1];
                    second = isolated[i];
                    desired_edges.push_back(make_pair(first, second));
                }

                if (sink_amount != 0) {
                    first = sinks_sorted[sink_amount - 1];
                    second = isolated[0];
                    desired_edges.push_back(make_pair(first, second));
                }
                if (source_amount != 0) {
                    first = isolated[isolated_amount - 1];
                    second = sources_sorted[0];
                    desired_edges.push_back(make_pair(first, second));
                }
            }

        }
        out << answer_a << endl;
        if (components.size() == 1) {
            ans = 0;
        }
        else {
            ans = desired_edges.size();
        }
        out << ans << endl;
        for (int i = 0; i < ans; i++) {
            out << desired_edges[i].first + 1 << " " << desired_edges[i].second + 1 << endl;
        }
        out.close();
    };
    vector<int> order;
    vector<int> representators;
    vector<int> unique_representators;


    vector<int> sinks;
    vector<int> sources;
    vector<int> temporary;
    vector<int> isolated;

    vector<bool> marked;

    vector<int> sources_sorted;
    vector<int> sinks_sorted;

    vector<bool> added_sources_sorted;
    vector<bool> added_sinks_sorted;

    vector<pair<int, int>> desired_edges;



    const int MAX_AMOUNT = 101;

    bool sink_not_found = true;
    int w = -1;

    void dfs1(vector<vector<int>>& net, int v, vector<char>& used) {
        used[v] = true;
        for (size_t i=0; i<net[v].size(); ++i)
            if (!used[ net[v][i] ])
                dfs1 (net, net[v][i], used);
        order.push_back (v);
    }
    void dfs2(int v, vector<char>& used, vector<int>& comp, vector<vector<int>>& t_net) {
        used[v] = true;
        comp.push_back(v);
        for (int i = 0; i < t_net[v].size(); i++) {
            if (!used[t_net[v][i]]) {
                dfs2(t_net[v][i], used, comp, t_net);
            }
        }
    }

    void search(vector<vector<int>>& net, int x, vector<vector<int>>& merged_graph) {
        if (!marked[x]) {
            bool is_sink = false;
            for (int i = 0; i < sinks.size() && !is_sink; i++) {
                if (x == sinks[i]) {
                    is_sink = true;
                    break;
                }
            }
            if (is_sink) {
                w = x;
                sink_not_found = false;
            }
            marked[x] = true;
            for (int i = 0; i < net.size(); i++) {
                if (merged_graph[x][i] == 1) {
                    if (sink_not_found) {
                        search(net, i, merged_graph);
                    }
                }
            }
        }
    }

    int strong_connectivity_sort(vector<vector<int>>& net, vector<vector<int>>& merged_graph) {
        for (int k = 0; k < marked.size(); k++) {
            marked[k] = false;
        }

        int appropriate = 0;

        while (!temporary.empty()) {
            int v = temporary.back();
            if (!marked[v]) {
                w = -1;
                sink_not_found = true;
                search(net, v, merged_graph);
                if (w != -1) {
                    appropriate++;
                    sources_sorted.push_back(v);
                    sinks_sorted.push_back(w);
                    added_sources_sorted[v] = true;
                    added_sinks_sorted[w] = true;
                }
            }

            temporary.pop_back();
        }

        return appropriate;
    }

};

bool compareOutput() {
    ifstream fs1("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/out.txt"),
    fs2("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/github_out.txt");
    vector<int> n1, n2;
    int tmp;
    int k = 0;
    while (fs1 >> tmp && k < 2) {
        n1.push_back(tmp);
        k++;
    }
    fs1.close();
    k = 0;
    while (fs2 >> tmp && k < 2) {
        n2.push_back(tmp);
        k++;
    }
    fs1.close();
    if(n1.size() != n2.size())
        return false;
    for(int i = 0; i < n1.size(); i++)
        if(n1[i] != n2[i])
            return false;
    return true;
}



void generateTest(int vertices) {
    srand(time(nullptr));
    ofstream out("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt");
    out << vertices << '\n';
    unordered_set<int> to;
    int edges, v;
    for (int i = 1; i <= vertices; ++i) {
        edges = rand() % (int)(vertices / 5);
        while (to.size() != edges) {
            v = (rand() % vertices) + 1;
            if (v != i)
                to.insert(v);
        }
        for(auto& ver : to) {
            out << ver << ' ';
        }
        out << 0 << '\n';
        to.clear();
    }
    out.close();
}

int main() {
    ofstream fs1("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/out.txt"),
            fs2("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/github_out.txt");
    fs1.close();
    fs2.close();
    for (int test = 0; test < 1; test++) {
        generateTest(15);
        Solution1();
        Solution2();
        if (!compareOutput()) {
            cout << "ответы не совпадают на тесте " << test + 1 << endl;
            return 1;
        }
    }
    cout << "ответы совпадают" << endl;
    return 0;
}