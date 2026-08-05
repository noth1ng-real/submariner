import sys
from Crypto.Cipher import AES
from os import urandom
import hashlib

KEY = urandom(16)

def pad(data):
    padlen = AES.block_size - (len(data) % AES.block_size)
    return data + bytes([padlen]) * padlen

def aesenc(plaintext, key):
    k = hashlib.sha256(key).digest()
    iv = b'\x00' * 16
    plaintext = pad(plaintext)
    cipher = AES.new(k, AES.MODE_CBC, iv)

    return cipher.encrypt(plaintext)

try:
    plaintext = open(sys.argv[1], "rb").read()
    ciphertext = aesenc(plaintext, KEY)

    print('AESkey[] = {' + ', '.join(f'0x{x:02x}' for x in KEY) + '};')
    print('payload[] = {' + ', '.join(f'0x{x:02x}' for x in ciphertext) + '};')
except Exception as e:
    print(f"Usage: {sys.argv[0]} <raw payload file>\n")
    import traceback
    traceback.print_exc()