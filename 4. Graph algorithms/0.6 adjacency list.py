if __name__ == "__main__":
	with open("input.txt") as input:
		vertices, edges = [int(item) for item in input.readline().split()]
		output_matrix = [[]*vertices for _ in range(vertices)]
		for line in range(edges):
			i, j = [int(item) - 1 for item in input.readline().split()]
			output_matrix[i].append(j + 1)
			output_matrix[j].append(i + 1)

	with open("output.txt", "w") as output:
		for i in range(vertices):
			output.write(str(len(output_matrix[i])) + " ")
			for element in output_matrix[i]:
				output.write(str(element) + " ")
			output.write('\n')
