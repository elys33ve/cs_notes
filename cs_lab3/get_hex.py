l = ""
x = input("str: ")

for i in x:
    l += f"{ord(i)} "
print(l)    # ascii value

h = " ".join("{:02x}".format(ord(c)) for c in x)
print(h)    # hex value
