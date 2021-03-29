//
// Created by akim on 04.03.2021.
//

#include <iostream>
#include <cstring>

using namespace std;

void createTree(long long*& a, long long*& tree, int i, int tl, int tr) {
    if (tr - tl == 1)
        tree[i] = a[tl];
    else {
        createTree(a, tree, 2 * i, tl, (tl + tr) / 2);
        createTree(a, tree, 2 * i + 1, (tl + tr) / 2, tr);
        tree[i] = tree[2 * i] + tree[2 * i + 1];
    }
}

void Add(long long*& array, long long*& tree, int a_size, int index, int num) {
    int i = 1, m, tl = 0, tr = a_size;
    tree[i] += num;
    while(tr > tl) {
        m = (tl + tr) / 2;
        if(m == tl)
            break;
        i *= 2;
        if(index >= m) {
            tl = m;
            i++;
        }
        else
            tr = m;
        tree[i] += num;
    }
}

void binSearch(long long*& tree, int left, int right, int i, int tl, int tr, long long& sum) {
    if(tl == left && tr == right) {
        sum += tree[i];
        return;
    }
    int m = (tl + tr) / 2;
    if(m == tl)
        return;
    if(left < m && right > m) {
        binSearch(tree, left, m, i, tl, tr, sum);
        binSearch(tree, m, right, i, tl, tr, sum);
    }
    else {
        i *= 2;
        if(left < m && right <= m)
            tr = m;
        else {
            tl = m;
            i++;
        }
        binSearch(tree, left, right, i, tl, tr, sum);
    }
}

long long findSum(long long*& tree, int a_size, int left, int right) {
    int i = 1, tl = 0, tr = a_size;
    long long sum = 0;
    binSearch(tree, left, right, i, tl, tr, sum);
    return sum;
}

int main() {
    int n, el, req_num, n1, n2;
    char* request = new char[10];
    cin >> n;
    auto *tree = new long long[4*n + 1];
    auto *array = new long long[n];
    for (int i = 0; i < n; ++i) {
        cin >> el;
        array[i] = el;
    }
    createTree(array, tree, 1, 0, n);

    cin >> req_num;
    for (int i = 0; i < req_num; ++i) {
        cin >> request >> n1 >> n2;
        if(!strcmp(request, "Add"))
            Add(array, tree, n, n1, n2);
        else
            cout << findSum(tree, n, n1, n2) << '\n';
    }
    return 0;
}