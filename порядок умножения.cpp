//
// Created by akim on 15.02.2021.
//
#include <iostream>
using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int s;
    cin >> s;
    int F[s + 1][s + 1], n[s + 1], m[s + 1];
    for(int i = 1; i <= s; i++)
        cin >> n[i] >> m[i];

    for(int i = 1; i <= s; i++)
        F[i][i] = 0;
    for(int p = 1; p <= s - 1; p++)
        for(int i = 1; i <= s - p; i++) {
            int x = INT32_MAX;
            int j = i + p;
            for(int k = i; k <= j - 1; k++) {
                if(x > (F[i][k] + F[k + 1][j] + n[i]*m[k]*m[j]))
                    x = F[i][k] + F[k + 1][j] + n[i]*m[k]*m[j];
            }
            F[i][j] = x;
        }
    cout << F[1][s];
    return 0;
}