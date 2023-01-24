from Crypto.PublicKey import RSA

key=RSA.generate(1024)
file_out=open("private.pem",'wb+')
file_out.write(key.exportKey('PEM'))
file_out.close()

new=key.publickey()
file_output=open("public.pem",'wb')
file_output.write(new.exportKey('PEM'))
file_output.close()
