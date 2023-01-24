import hashlib

HASH_NAME = "sha1"

txt = input("패스워드를 입력하세요: ")

text = txt.encode('utf-8')
sha1 = hashlib.new(HASH_NAME)
sha1.update(text)
result = sha1.hexdigest()
c=[]
for i in range(16):
    a=result[2*i:2*i+2]
    b=int(a,16)
    c.append(b)
keyname=input("키를 저장할 파일의 이름을 입력하세요: ")
with open(keyname,mode='w+b') as file:
    file.write(bytes(c))
