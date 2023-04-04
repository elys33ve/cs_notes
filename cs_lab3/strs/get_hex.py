# convert str to hex
""" quick python script to run from terminal that takes str input
    and converts it to hex values of each character in the format for
    this lab
"""


asciival = ""
x = input("str: ")

# get ascii
for i in x:
    asciival += f"{ord(i)} "
asciival = asciival[:-1]

# get hex
hexval = " ".join("{:02x}".format(ord(c)) for c in x)


# outputs
a = f"({asciival})"             # ascii value
h = f"{hexval}"                 # hex value

if len(asciival) > 25:  # format a bit
    a += '\n'
print(a + "      " + h)