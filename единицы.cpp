//
// Created by akim on 15.02.2021.
//
#include <iostream>
using namespace std;

unsigned long long R = 1000000007;

unsigned long long bin_pow(unsigned long long base, unsigned long long p) {
    if (p == 1) {
        return base;
    }
    if (p % 2 == 0) {
        unsigned long long t = bin_pow(base, p / 2);
        return t * t % R;
    } else {
        return bin_pow(base, p - 1) * base % R;
    }
}

unsigned long long factorial(unsigned long long n, unsigned long long lower=2) {
    unsigned long long answer = 1;
    for (unsigned long long i = lower; i <= n; ++i)
        answer = (answer * i) % R;
    return answer;
}

unsigned long long combinations(unsigned long long n, unsigned long long k) {
    return (factorial(n, n - k + 1) * bin_pow(factorial(k), R - 2)) % R;
}

int main() {
    unsigned long long N, K;
    cin >> N >> K;
    cout << combinations(N, K) << '\n';

    return 0;
}
