//
// Created by akim on 08.03.2021.
//
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#define NIL -1

using namespace std;


const int MAX = 100;
vector<int> g[MAX];
bool used[MAX];
vector<int> component;

void dfs(int v) {
    used[v] = true;
    component.push_back(v);
    for (int to : g[v])
        if (!used[to])
            dfs(to);
}
void findCC(int n) {
    for (int i = 1; i <= n; ++i)
        used[i] = false;
    for (int i = 1; i <= n; ++i)
        if (!used[i]) {
            component.clear();
            dfs(i);

            cout << "Component:";
            for (int j : component)
                cout << ' ' << j;
            cout << endl;
        }
}

class Graph {
    int V;
    list<int> *adj;
    void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[]) {
        static int time = 0;

        disc[u] = low[u] = ++time;
        st->push(u);
        stackMember[u] = true;

        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            int v = *i;
            if (disc[v] == -1) {
                SCCUtil(v, disc, low, st, stackMember);
                low[u]  = min(low[u], low[v]);
            }
            else if (stackMember[v])
                low[u]  = min(low[u], disc[v]);
        }
        int w;
        if (low[u] == disc[u]) {
            while (st->top() != u) {
                w = st->top();
                cout << w << " ";
                stackMember[w] = false;
                st->pop();
            }
            w = (int) st->top();
            cout << w << "\n";
            stackMember[w] = false;
            st->pop();
        }
    }

public:
    explicit Graph(int V) {
        this->V = V;
        adj = new list<int>[V + 1];
    }
    void addEdge(int v, int w) { adj[v].push_back(w); }
    void SCC() {
        int *disc = new int[V + 1];
        int *low = new int[V + 1];
        bool *stackMember = new bool[V + 1];
        auto *st = new stack<int>();

        for (int i = 1; i <= V; i++) {
            disc[i] = NIL;
            low[i] = NIL;
            stackMember[i] = false;
        }

        for (int i = 1; i <= V; i++)
            if (disc[i] == NIL)
                SCCUtil(i, disc, low, st, stackMember);
    }
};

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/input.txt","r", stdin);
    int n, j = -1;
    cin >> n;
    Graph g1(n);
    for (int i = 1; i <= n; ++i) {
        while(true) {
            cin >> j;
            if(!j)
                break;
            g1.addEdge(i, j);
            g[i].push_back(j);
        }
    }
    findCC(n);
    g1.SCC();
    return 0;
}