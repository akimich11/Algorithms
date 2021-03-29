//
// Created by akim on 21/03/2021.
//
#include <iostream>

using namespace std;

string toString(int c) {
    switch (c) {
        case 0:
            return "chu";
        case 1:
            return "ka";
        default:
            return "pi";
    }
}

int main() {
    int N;
    unsigned long long K;
    cin >> N >> K;

    if(N == 1) {
        cout << toString(K - 1) << endl;
        return 0;
    }

    unsigned long long dp[N - 1][9];
    for(int i = 0; i < 9; ++i)
        dp[0][i] = 1;

    for (int i = 1; i < N - 1; ++i) {
        dp[i][0] = dp[i][3] = dp[i][6] = dp[i - 1][0] + dp[i - 1][1] + dp[i - 1][2];
        dp[i][2] = dp[i][5] = dp[i - 1][6] + dp[i - 1][7] + dp[i - 1][8];
        dp[i][1] = dp[i - 1][3] + dp[i - 1][4];
        dp[i][4] = dp[i - 1][3] + dp[i - 1][5];
        dp[i][7] = dp[i - 1][3] + dp[i - 1][4] + dp[i - 1][5];
        dp[i][8] = dp[i - 1][6] + dp[i - 1][7];
    }

    unsigned long long sum = 0;
    int i = N - 2, j = 0;
    while (j < 9 && sum + dp[i][j] < K) {
        sum += dp[i][j];
        j++;
    }
    int left, right;
    string answer = toString(j / 3) + toString(j % 3);

    while(i > 0 && sum != K) {
        i--;
        if(j == 0 || j == 3 || j == 6)
            left = 0, right = 3;
        else if(j == 2 || j == 5)
            left = 6, right = 9;
        else if(j == 1)
            left = 3, right = 5;
        else if(j == 7)
            left = 3, right = 6;
        else
            left = 6, right = 8;

        if(j != 4) {
            for(int k = left; k < right; ++k) {
                if(sum + dp[i][k] < K)
                    sum += dp[i][k];
                else {
                    j = k;
                    answer += toString(j % 3);
                    if(sum + dp[i][k] == K && dp[i][k] == 1)
                        sum += dp[i][k];
                    break;
                }
            }
        }
        else {
            if(sum + dp[i][3] < K)
                sum += dp[i][3];
            else {
                j = 3;
                answer += toString(j % 3);
                if(sum + dp[i][3] == K && dp[i][3] == 1)
                    sum += dp[i][3];
                continue;
            }
            if(sum + dp[i][5] < K)
                sum += dp[i][5];
            else {
                j = 5;
                answer += toString(j % 3);
                if(sum + dp[i][5] == K && dp[i][5] == 1)
                    sum += dp[i][5];
            }
        }
    }

    cout << answer << endl;
    return 0;
}