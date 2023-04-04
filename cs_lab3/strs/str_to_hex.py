# convert string to hex (file input/output)
""" takes str/plaintext contents from strs.txt, converts each char in str
    to hex value separated by space and ending with null, then outputs results
    to hex.txt file
    (not perfect but it works for this lab)
"""

strings = "strs.txt"
hexfile = "hex.txt"
#exploits = "exploits.txt"


test = False        # true = print to screen, false = write to file

### string to hex
def str_to_hex(pr=False):
    inp_file = open(strings, 'r')       # get input
    string = inp_file.read()
    inp_file.close()

    string = " ".join("{:02x}".format(ord(c)) for c in string)
    string = string.replace("0a", "") + ' 00\n'

    if pr == False:
        out_file = open(hexfile, 'w')       # output to file
        out_file.write(string)
        out_file.close()
    else:
        print(string)

str_to_hex(test)






"""
#(i got bored but this was unnecessary to begin with since hex2strs was provided)

### hex to string
def hex_to_str(pr=False):
    inp_file = open(hexs, 'r')       # get input
    string = inp_file.read()
    inp_file.close()

    x = ""
    newstr = []                                 # format str
    string = 'x' + string.replace(" ", "x")
    for i in range(len(string)-1):
        x += string[i]
        if string[i+1] == 'x' and i != 1:
            newstr.append('0' + x)
            x = ""
    if 'x' in x and len(x) == 3:
        newstr.append('0' + x)
    
    hex_vals = []                               # convert to hex
    for i in newstr:
        intval = int(i, 16)
        hexval = hex(intval)
        hex_vals.append(hexval)
        print(chr(intval))
    
    print(int('a', ))

    if pr == False:
        out_file = open(strings, 'w')       # output to file
        out_file.write(string)
        out_file.close()
    else:
        print(string)
"""