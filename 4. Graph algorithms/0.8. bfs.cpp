//
// Created by akim on 29/03/2021.
//
#include<iostream>
#include<queue>
using namespace std;

bool* used;
bool** input_matrix;
int* order;
queue<int> q;
int n, current_vertex = 0;

void bfs(int start) {
    used[start] = true;
    current_vertex++;
    order[start] = current_vertex;
    q.push(start);
    int v;
    while(!q.empty()) {
        v = q.front();
        q.pop();
        for(int i = 0; i < n; i++)
            if(input_matrix[v][i] && !used[i]) {
                used[i] = true;
                current_vertex++;
                order[i] = current_vertex;
                q.push(i);
            }
    }
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/output.txt", "w", stdout);
    cin >> n;
    used = new bool[n]{false};
    order = new int[n]{0};
    input_matrix = new bool*[n];
    bool item;
    for (int i = 0; i < n; ++i) {
        input_matrix[i] = new bool[n]{false};
        for (int j = 0; j < n; j++) {
            cin >> item;
            input_matrix[i][j] = item;
        }
    }
    for (int i = 0; i < n; ++i)
        if (!used[i])
            bfs(i);
    for (int i = 0; i < n; ++i)
        cout << order[i] << ' ';
    return 0;
}
