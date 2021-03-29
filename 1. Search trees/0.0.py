def count_sum():
    node_set = set()
    with open("input.txt") as f:
        for line in f.readlines():
            if line.strip('\n') != '':
                node_set.add(int(line))
    with open("output.txt", "w") as f:
        f.write(str(sum(node_set)))


if __name__ == '__main__':
    count_sum()
