//
// Created by akim on 18.02.2021.
//
#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/output.txt", "w", stdout);

    int n, m, new_len;
    string city1, city2;
    unordered_map<string, int> cities;
    cin >> n >> m;
    bool **paths = new bool *[n];
    int **path_lens = new int *[n];

    for (int i = 0; i < n; ++i) {
        paths[i] = new bool[n]{};
        path_lens[i] = new int[n]{};
        cin >> city1;
        cities[city1] = i;
    }

    for (int i = 0; i < m; ++i) {
        cin >> city1 >> city2;
        paths[cities[city1]][cities[city2]] = true;
        paths[cities[city2]][cities[city1]] = true;
    }

    path_lens[n - 1][n - 1] = 1;
    for (int i = n - 2; i >= 0; --i) {
        for (int j = n - 1; j >= i; --j) {
            for (int k = n - 1; k > i; --k) {
                if(!paths[k][i])
                    continue;

                if (i == j && path_lens[k][j] > path_lens[i][j])
                    path_lens[i][j] = path_lens[k][j];

                else if(i != j && (j == n - 1 || k != j)) {
                    new_len = path_lens[k][j] + 1;
                    if(new_len != 1 && new_len > path_lens[i][j]) {
                        path_lens[i][j] = new_len;
                        path_lens[j][i] = new_len;
                    }
                }
            }
        }
    }
    if(**path_lens)
        cout << **path_lens;
    else
        cout << "No solution";

    return 0;
}