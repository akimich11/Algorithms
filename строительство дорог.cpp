//
// Created by akim on 22/03/2021.
//
#include <iostream>
using namespace std;

class DSU {
private:
    int *size, *parent, components;
public:
    explicit DSU(int n) {
        size = new int[n]{1};
        parent = new int[n];
        for (int i = 0; i < n; ++i)
            parent[i] = i;
        components = n;
    }
    int getComponents() const {
        return components;
    }
    int findSet(int x) {
        if(x == parent[x])
            return x;
        parent[x] = findSet(parent[x]);
        return parent[x];
    }
    void unionSets(int x, int y) {
        x = findSet(x);
        y = findSet(y);
        if(x != y) {
            if(size[x] < size[y])
                swap(x, y);
            parent[y] = x;
            size[x] += size[y];
            components--;
        }
    }
};

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/output.txt", "w", stdout);
    int N, q, c1, c2;
    cin >> N >> q;
    DSU cities(N);
    for (int i = 0; i < q; ++i) {
        cin >> c1 >> c2;
        cities.unionSets(c1 - 1, c2 - 1);
        cout << cities.getComponents() << '\n';
    }
    return 0;
}
