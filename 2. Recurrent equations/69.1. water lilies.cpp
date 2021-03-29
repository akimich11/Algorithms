//
// Created by akim on 15.02.2021.
//
#include <iostream>
using namespace std;

int optimalWay(int lilies_number) {
    int *array = new int[lilies_number];
    int *optimal = new int[lilies_number];

    cin >> array[0];
    optimal[0] = array[0];
    if (lilies_number == 1)
        return optimal[0];

    cin >> array[1];
    if (lilies_number == 2)
        return -1;

    cin >> array[2];
    optimal[2] = array[0] + array[2];
    if (lilies_number == 3)
        return optimal[2];

    cin >> array[3];
    optimal[3] = array[0] + array[3];
    if (lilies_number == 4)
        return optimal[3];

    cin >> array[4];
    optimal[4] = array[0] + array[2] + array[4];
    if(lilies_number == 5)
        return optimal[4];

    for (int i = 5; i < lilies_number; ++i) {
        cin >> array[i];
        optimal[i] = max(optimal[i - 2], optimal[i - 3]) + array[i];
    }
    return optimal[lilies_number - 1];
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int lilies_number;
    cin >> lilies_number;
    cout << optimalWay(lilies_number);

    return 0;
}
