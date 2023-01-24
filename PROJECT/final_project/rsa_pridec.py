from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

name=input("개인키로 복호화할 키 파일을 입력하세요: ")

with open(name,'r+b')as file:
    ade=file.read()
    
private_key=RSA.import_key(open('private.pem').read())
cipher=PKCS1_OAEP.new(private_key)

dec=cipher.decrypt(ade)
print(dec)

with open(name,'w+b')as file:
    file.write(dec)
