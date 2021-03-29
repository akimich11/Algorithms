//
// Created by akim on 22/03/2021.
//
#include <iostream>
#include <vector>

using namespace std;

class DSU {
private:
    int *size, *parent;
public:
    int components;
    explicit DSU(int& n) {
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
    int N, q, m, c1, c2, *crashes;
    bool* is_crashed;
    vector<pair<int, int>> roads;

    scanf("%d %d %d", &N, &m, &q);
    roads.reserve(m);
    crashes = new int[q];
    is_crashed = new bool[m]{false};
    DSU cities(N);

    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &c1, &c2);
        roads.emplace_back(c1 - 1, c2 - 1);
    }
    for (int i = 0; i < q; ++i) {
        scanf("%d", &c1);
        crashes[i] = c1 - 1;
        is_crashed[crashes[i]] = true;
    }

    for (int i = 0; i < m; ++i)
        if(!is_crashed[i])
            cities.unionSets(roads[i].first, roads[i].second);

    int earthquakes = 0;
    for(int i = q - 1; i > -1; --i) {
        if(cities.components == 1)
            break;
        cities.unionSets(roads[crashes[i]].first, roads[crashes[i]].second);
        earthquakes++;
    }
    for (int i = 0; i < q - earthquakes; ++i)
        printf("%c", '1');
    for (int i = 0; i < earthquakes; ++i)
        printf("%c", '0');
    return 0;
}