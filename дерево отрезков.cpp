//
// Created by akim on 04.03.2021.
//

#include <iostream>
#include <cstring>
using namespace std;

void createTree(int*& a, int*& t, int v, int tl, int tr) {
    if (tr - tl == 1)
        t[v] = a[tl];
    else {
        createTree(a, t, 2 * v, tl, (tl + tr) / 2);
        createTree(a, t, 2 * v + 1, (tl + tr) / 2, tr);
        t[v] = t[2 * v] + t[2 * v + 1];
    }
}

void Add(int*& tree, int i, int left, int right, int index, int num) {
    int m = (left + right) / 2;
    if (m == left) {
        tree[i] += num;
        return;
    }
    if (index >= m)
        Add(tree, 2 * i + 1, m, right, index, num);
    else
        Add(tree, 2 * i, left, m, index, num);
    tree[i] = tree[2 * i] + tree[2 * i + 1];
}

int findSum(int*& tree, int left, int right, int i, int tl, int tr) {
    if (tl == left && tr == right)
        return tree[i];
    int m = (tl + tr) / 2;
    if (right <= m)
        return findSum(tree, left, right, 2 * i, tl, m);
    if (left >= m)
        return findSum(tree, left, right, 2 * i + 1, m, tr);

    return findSum(tree, left, m, i, tl, tr) +
           findSum(tree, m, right, i, tl, tr);
}

int main() {
    int n, el, req_num, n1, n2;
    char* request = new char[10];
    cin >> n;
    int* tree = new int[4 * n + 1];
    int* array = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> el;
        array[i] = el;
    }
    createTree(array, tree, 1, 0, n);

    cin >> req_num;
    for (int i = 0; i < req_num; ++i) {
        cin >> request >> n1 >> n2;
        if (!strcmp(request, "Add"))
            Add(tree, 1, 0, n, n1, n2);
        else
            cout << findSum(tree, n1, n2, 1, 0, n) << '\n';
    }
    return 0;
}