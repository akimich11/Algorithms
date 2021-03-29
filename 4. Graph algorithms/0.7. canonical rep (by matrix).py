if __name__ == "__main__":
	with open("input.txt") as input:
		vertices = int(input.readline())
		output_matrix = [0]*vertices
		for i in range(vertices):
			j = 0
			for n in input.readline().split():
				if n == '1':
					output_matrix[j] = i + 1
				j += 1
				
	with open("output.txt", "w") as output:
		for i in range(vertices):
			output.write(str(output_matrix[i]) + " ")
