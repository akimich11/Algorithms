//
// Created by akim on 23.02.2021.
//
#include <iostream>
using namespace std;

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/output.txt", "w", stdout);
    int n, element;
    cin >> n;
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        cin >> element;
        a[i] = element;
        if((i % 2 == 0 && a[(i - 1)/2] > element) || (i % 2 == 1 && a[i/2] > element)) {
            cout << "No\n";
            return 0;
        }
    }
    cout << "Yes\n";
    return 0;
}