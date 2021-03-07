//
// Created by akim on 01.03.2021.
//
#include <iostream>
using namespace std;

int min3(int a, int b, int c) {
    return min(a, min(b, c));
}


int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/output.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int a[n], b[n], c[n], dp[n];
    for (int i = 0; i < n; ++i)
        cin >> a[n - 1 - i] >> b[n - 1 - i] >> c[n - 1 - i];

    dp[0] = a[0];
    dp[1] = min(a[0] + a[1], b[1]);
    dp[2] = min3(c[2], b[2] + dp[0], a[2] + dp[1]);
    for (int i = 3; i < n; ++i)
        dp[i] = min3(c[i] + dp[i - 3], b[i] + dp[i - 2], a[i] + dp[i - 1]);
    cout << dp[n - 1];
    return 0;
}

