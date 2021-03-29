def to_string(c):
    if c == 0:
        return "chu"
    if c == 1:
        return "ka"
    return "pi"


if __name__ == "__main__":
    N, K = (int(item) for item in input().split())
    if N == 1:
        print(to_string(K - 1))
        exit(0)

    dp = [[0] * 9 for _ in range(N - 1)]
    for i in range(9):
        dp[0][i] = 1

    for i in range(1, N - 1):
        dp[i][0] = dp[i][3] = dp[i][6] = dp[i - 1][0] + dp[i - 1][1] + dp[i - 1][2]
        dp[i][2] = dp[i][5] = dp[i - 1][6] + dp[i - 1][7] + dp[i - 1][8]
        dp[i][1] = dp[i - 1][3] + dp[i - 1][4]
        dp[i][4] = dp[i - 1][3] + dp[i - 1][5]
        dp[i][7] = dp[i - 1][3] + dp[i - 1][4] + dp[i - 1][5]
        dp[i][8] = dp[i - 1][6] + dp[i - 1][7]

    summ = 0
    i = N - 2
    j = 0
    while j < 9 and summ + dp[i][j] < K:
        summ += dp[i][j]
        j += 1
    left = 0
    right = 0
    answer = to_string(j // 3) + to_string(j % 3)

    while i > 0 and summ != K:
        i -= 1
        if j == 0 or j == 3 or j == 6:
            left = 0
            right = 3
        elif j == 2 or j == 5:
            left = 6
            right = 9
        elif j == 1:
            left = 3
            right = 5
        elif j == 7:
            left = 3
            right = 6
        else:
            left = 6
            right = 8

        if j != 4:
            for k in range(left, right):
                if summ + dp[i][k] < K:
                    summ += dp[i][k]
                else:
                    j = k
                    answer += to_string(j % 3)
                    if summ + dp[i][k] == K and dp[i][k] == 1:
                        summ += dp[i][k]
                    break

        else:
            if summ + dp[i][3] < K:
                summ += dp[i][3]
            else:
                j = 3
                answer += to_string(j % 3)
                if summ + dp[i][3] == K and dp[i][3] == 1:
                    summ += dp[i][3]
                continue

            if summ + dp[i][5] < K:
                summ += dp[i][5]
            else:
                j = 5
                answer += to_string(j % 3)
                if summ + dp[i][5] == K and dp[i][5] == 1:
                    summ += dp[i][5]

    print(answer)
