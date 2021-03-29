if __name__ == "__main__":
    with open("input.txt") as f:
        string = f.read().strip('\n')
    n = len(string)
    if n == 1:
        with open("output.txt", "w") as f:
            f.write('1\n' + string)
    else:
        F = []
        for i in range(n - 1):
            F.append([0] * n)
            F[i][i] = 1
            if string[i] == string[i + 1]:
                F[i][i + 1] = 2
            else:
                F[i][i + 1] = 1
        F.append([0] * n)
        F[n - 1][n - 1] = 1

        for j in range(2, n):
            for i in range(n - j):
                if string[i] == string[i + j]:
                    F[i][i + j] = F[i + 1][i + j - 1] + 2
                else:
                    F[i][i + j] = max(F[i + 1][i + j], F[i][i + j - 1])

        out_str = ""
        i = 0
        j = n - 1
        while i < j:
            if string[i] == string[j]:
                out_str += string[i]
                i += 1
                j -= 1
            elif F[i][j] == F[i + 1][j]:
                i += 1
            elif F[i][j] == F[i][j - 1]:
                j -= 1

            if F[i][j] == 1:
                out_str += string[i]
                break

        if F[0][n - 1] % 2 == 0:
            out_str = out_str + out_str[::-1]
        else:
            out_str = out_str + (out_str[::-1])[1::]

        with open("output.txt", "w") as f:
            f.write(str(F[0][n - 1]) + '\n' + out_str)