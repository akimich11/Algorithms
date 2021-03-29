if __name__ == "__main__":
	with open("input.txt") as input:
		vertices, edges = [int(item) for item in input.readline().split()]
		output_matrix = [[0]*vertices for _ in range(vertices)]
		for line in range(edges):
			i, j = [int(item) - 1 for item in input.readline().split()]
			output_matrix[i][j] = output_matrix[j][i] = 1

	with open("output.txt", "w") as output:
		for i in range(vertices):
			for j in range(vertices):
				output.write(str(output_matrix[i][j]) + " ")
			output.write('\n')
