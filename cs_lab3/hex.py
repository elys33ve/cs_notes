x = input("str: ")
h = " ".join("{:02x}".format(ord(c)) for c in x)

filename = "strs.txt"
f = open(filename, 'w')
f.write(h + '\n')
f.close()