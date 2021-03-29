//
// Created by akim on 22/03/2021.
//
#include <iostream>
using namespace std;

class DSU {
private:
    int *size, *parent;
public:
    int components;
    explicit DSU(int n) {
        size = new int[n];
        parent = new int[n];
        for (int i = 0; i < n; ++i) {
            size[i] = 1;
            parent[i] = i;
        }
        components = n;
    }
    int findSet(int& x) {
        if(x == parent[x])
            return x;
        parent[x] = findSet(parent[x]);
        return parent[x];
    }
    void unionSets(int x, int y) {
        x = findSet(x);
        y = findSet(y);
        if(x != y) {
            if(size[y] > size[x])
                swap(x, y);
            parent[x] = y;
            size[y] += size[x];
            components--;
        }
    }
};

int main() {
    freopen("D:\\Workspace\\CLion\\Algorithms_local\\input.txt", "r", stdin);
    freopen("D:\\Workspace\\CLion\\Algorithms_local\\output.txt", "w", stdout);
    int N, q, c1, c2;
    scanf("%d %d", &N, &q);
    DSU cities(N);
    for (int i = 0; i < q; ++i) {
        scanf("%d %d", &c1, &c2);
        cities.unionSets(c1 - 1, c2 - 1);
        cout << cities.components << '\n';
    }
    return 0;
}