n = 0
i = 1

used = []
matrix = []
marks = []


def dfs(start):
	global i
	used[start] = True
	marks[start] = i
	i += 1
	for v in range(n):
		if matrix[start][v] == 1 and not used[v]:
			dfs(v)


if __name__ == "__main__":
	with open("input.txt") as f:
		n = int(f.readline())
		used = [False] * n
		marks = [0] * n
		matrix = [[0] * n for _ in range(n)]
		for v in range(n):
			matrix[v] = [int(item) for item in f.readline().split()]
		for v in range(n):
			if not used[v]:
				dfs(v)
	with open("output.txt", "w") as f:
		for v in range(n):
			f.write(str(marks[v]) + " ")
