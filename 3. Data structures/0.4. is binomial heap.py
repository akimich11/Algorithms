if __name__ == '__main__':
    with open("input.txt") as f:
        number = int(f.read())
    with open("output.txt", "w") as f:
        if number == 0:
            f.write("-1")
        else:
            binary = ""
            while number != 0:
                binary += str(number % 2)
                number //= 2

            i = 0
            for char in binary:
                if char == '1':
                    f.write(str(i) + '\n')
                i += 1
