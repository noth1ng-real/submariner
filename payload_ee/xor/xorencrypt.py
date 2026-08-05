import sys

KEY = b'mysecretkey'

def xor(data, key):
    output = bytearray()

    for i in range(len(data)):
        output.append(data[i] ^ key[i % len(key)])

    return output

def printCipherText(ciphertext):
    print('payload[] = {' + ', '.join(f'0x{x:02x}' for x in ciphertext) + '};')

try:
    plaintext = open(sys.argv[1], "rb").read()
    printCipherText(xor(plaintext, KEY))
except Exception as e:
    print(f"Usage: {sys.argv[0]} <raw payload file>\n")
    import traceback
    traceback.print_exc()