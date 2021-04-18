//
// Created by akim on 02/04/2021.
//

#include <iostream>
#define INT long long
using namespace std;


INT fact, N, R = 1000000007;

INT bin_pow(INT a, INT n) {
    INT res = 1;
    while (n) {
        if (n & 1)
            res = (res * a) % R;
        a = (a * a) % R;
        n >>= 1;
    }
    return res;
}

INT factorial(INT n) {
    INT answer = 1;
    for (INT i = 2; i <= n; ++i)
        answer = (answer * i) % R;
    return answer;
}

void createTree(INT*& tree, INT i, INT tl, INT tr) {
    if (tr - tl == 1)
        tree[i] = fact;
    else {
        createTree(tree, 2 * i, tl, (tl + tr) / 2);
        createTree(tree, 2 * i + 1, (tl + tr) / 2, tr);
        tree[i] = (tree[2 * i] + tree[2 * i + 1]) % R;
    }
}

void subtract(INT*& tree, INT index, INT num) {
    INT i = 1, m, tl = 0, tr = N;
    tree[i] = (tree[i] - num) % R;
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
        tree[i] = (tree[i] - num) % R;
    }
}

void binSearch(INT *& tree, INT left, INT right, INT i, INT tl, INT tr, INT& sum) {
    if(tl == left && tr == right) {
        sum += tree[i];
        sum %= R;
        return;
    }
    INT m = (tl + tr) / 2;
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
INT findSum(INT*& tree, INT left, INT right) {
    INT i = 1, tl = 0, tr = N, s = 0;
    binSearch(tree, left, right, i, tl, tr, s);
    return s;
}

int main() {
    INT k;
    cin >> N;
    fact = factorial(N - 1);
    auto *tree = new INT[4 * N + 1];

    auto *combination = new INT[N];
    for(INT i = 0; i < N; ++i) {
        cin >> k;
        combination[i] = k - 1;
    }
    createTree(tree, 1, 0, N);
    INT denominator = 1;
    INT sum = (1 + findSum(tree, 0, combination[0])) % R;
    subtract(tree, combination[0], fact);
    for (INT i = 1; i < N - 1; ++i) {
        denominator = (denominator * (N - i)) % R;
        sum = (sum + findSum(tree, 0, combination[i]) * bin_pow(denominator, R - 2)) % R;
        if(sum < 0)
            sum += R;
        subtract(tree, combination[i], fact);
    }
    cout << sum << endl;
    return 0;
}