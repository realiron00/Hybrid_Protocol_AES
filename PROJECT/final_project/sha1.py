import hashlib

def sha1_for_largefile(filepath, blocksize=8192):
    sha_1 = hashlib.sha1()
    try:
        f = open(filepath, "rb")
    except IOError as e:
        print("file open error", e)
        return
    while True:
        buf = f.read(blocksize)
        if not buf:
            break
        sha_1.update(buf)
    return sha_1.hexdigest()

name=input("파일을 입력하세요 : ")
a=sha1_for_largefile(name)
keyname=input("해시값을 저장할 파일의 이름을 입력하세요: ")
with open(keyname,mode='w') as file:
    file.write(a)
