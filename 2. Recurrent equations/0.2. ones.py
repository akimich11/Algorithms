def factorial(n):
    answer = 1
    for i in range(2, n + 1):
        answer *= i
        answer = answer % (10**9 + 7)
    return answer


def combinations(n, k):
    return factorial(n) // (factorial(k) * factorial(n - k))


if __name__ == "__main__":
    N, K = [int(item) for item in input().split()]
    print(combinations(N, K) % (10**9 + 7))
