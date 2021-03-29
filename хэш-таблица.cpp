//
// Created by akim on 25.03.2021.
//
#include <iostream>
using namespace std;

int m, c, n;

void insert(int* table, int x) {
    int i = 0, index = ((x % m) + c * i) % m;
    while(table[index] != -1) {
        index = ((x % m) + c * i) % m;
        i++;
        if(table[index] == x)
            return;
    }
    table[index] = x;
}

int main() {
    freopen("D:\\Workspace\\CLion\\Algorithms_local\\input.txt", "r", stdin);
    int x;
    cin >> m >> c >> n;
    int* hash_table = new int[m];
    for (int i = 0; i < m; ++i)
        hash_table[i] = -1;


    for (int j = 0; j < n; ++j) {
        cin >> x;
        insert(hash_table, x);
    }
    for (int j = 0; j < m; ++j)
        cout << hash_table[j] << " ";
    return 0;
}
