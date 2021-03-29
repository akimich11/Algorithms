if __name__ == "__main__":
	with open("input.txt") as input:
		vertices = int(input.readline())
		output_matrix = [0]*vertices
		for line in range(vertices - 1):
			v1, v2 = [int(item) - 1 for item in input.readline().split()]
			output_matrix[v2] = v1 + 1

	with open("output.txt", "w") as output:
		for i in range(vertices):
			output.write(str(output_matrix[i]) + " ")
