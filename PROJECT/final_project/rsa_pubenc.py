from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

name=input("공개키로 암호화할 키 파일을 입력하세요: ")

with open(name,'r+b')as file:
    ade=file.read()

public_key=RSA.import_key(open('public.pem').read())
cipher=PKCS1_OAEP.new(public_key)

enc=cipher.encrypt(ade)
print(enc)

with open(name,'w+b')as file:
    file.write(enc)
